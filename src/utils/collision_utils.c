#include "collision_utils.h"

bool intersect_sphere_point(sphere_collider_component *comp, Vector3 point) {
    float distance = 
        (point.x - comp->pos.x)*(point.x - comp->pos.x) +
        (point.y - comp->pos.y)*(point.y - comp->pos.y) +
        (point.z - comp->pos.z)*(point.z - comp->pos.z);
        
    return distance < comp->radius * comp->radius;
}

bool intersect_sphere_sphere(sphere_collider_component *compA, sphere_collider_component *compB) {
    transform_component *transform_a = get_component(compA->base.gameObject, transform_component, "transform_component");
    transform_component *transform_b = get_component(compB->base.gameObject, transform_component, "transform_component");
    float distance = 
        (transform_a->position.x - transform_b->position.x)*(transform_a->position.x - transform_b->position.x)+
        (transform_a->position.y - transform_b->position.y)*(transform_a->position.y - transform_b->position.y)+
        (transform_a->position.z - transform_b->position.z)*(transform_a->position.z - transform_b->position.z);
    
    float radiusSum = compA->radius + compB->radius;
    return distance <= radiusSum * radiusSum;
}

bool intersect_AABB_sphere(box_collider_component *compA, sphere_collider_component *compB) {
    transform_component *transform_a = get_component(compA->base.gameObject, transform_component, "transform_component");
    transform_component *transform_b = get_component(compB->base.gameObject, transform_component, "transform_component");
    Vector3 halfA = {
        transform_a->scale.x * 0.5f,
        transform_a->scale.y * 0.5f,
        transform_a->scale.z * 0.5f,
    };

    float x = max(transform_a->position.x - halfA.x, min(transform_b->position.x, transform_a->position.x + halfA.x));
    float y = max(transform_a->position.y - halfA.y, min(transform_b->position.y, transform_a->position.y + halfA.y));
    float z = max(transform_a->position.z - halfA.z, min(transform_b->position.z, transform_a->position.z + halfA.z));

    float distance =
        (x - transform_b->position.x)*(x - transform_b->position.x)+
        (y - transform_b->position.y)*(y - transform_b->position.y)+
        (z - transform_b->position.z)*(z - transform_b->position.z);

    return distance <= compB->radius * compB->radius;
}

bool intersect_AABB_point(box_collider_component *comp, Vector3 pos) {
    Vector3 half = {
        comp->scale.x * 0.5f,
        comp->scale.y * 0.5f,
        comp->scale.z * 0.5f
    };

    return (
        pos.x >= comp->pos.x - half.x &&
        pos.x <= comp->pos.x + half.x  &&
        pos.y >= comp->pos.y - half.y  &&
        pos.y <= comp->pos.y + half.y  &&
        pos.z >= comp->pos.z - half.z  &&
        pos.z <= comp->pos.z + half.z 
    );
}

bool intersect_AABB_AABB(box_collider_component *compA, box_collider_component *compB) {
    Vector3 halfA = {
        compA->scale.x * 0.5f,
        compA->scale.y * 0.5f,
        compA->scale.z * 0.5f
    };
    Vector3 halfB = {
        compB->scale.x * 0.5f,
        compB->scale.y * 0.5f,
        compB->scale.z * 0.5f
    };
    return (
        (compA->pos.x - halfA.x <= compB->pos.x + halfB.x && compA->pos.x + halfA.x >= compB->pos.x - halfB.x) &&
        (compA->pos.y - halfA.y <= compB->pos.y + halfB.y && compA->pos.y + halfA.y >= compB->pos.y - halfB.y) &&
        (compA->pos.z - halfA.z <= compB->pos.z + halfB.z && compA->pos.z + halfA.z >= compB->pos.z - halfB.z)    
    );
}