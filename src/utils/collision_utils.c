#include "collision_utils.h"

bool intersect_sphere_point(sphere_collider_component *comp, Vector3 point) {
    float distance = 
        (point.x - comp->pos.x)*(point.x - comp->pos.x) +
        (point.y - comp->pos.y)*(point.y - comp->pos.y) +
        (point.z - comp->pos.z)*(point.z - comp->pos.z);
        
    return distance < comp->radius * comp->radius;
}

bool intersect_sphere_sphere(sphere_collider_component *compA, sphere_collider_component *compB) {
    float distance = 
        (compA->pos.x - compB->pos.x)*(compA->pos.x - compB->pos.x)+
        (compA->pos.y - compB->pos.y)*(compA->pos.y - compB->pos.y)+
        (compA->pos.z - compB->pos.z)*(compA->pos.z - compB->pos.z);
    
    float radiusSum = compA->radius + compB->radius;
    
    return distance <= radiusSum * radiusSum;
}

bool intersect_AABB_sphere(box_collider_component *compA, sphere_collider_component *compB) {
    Vector3 halfA = {
        compA->scale.x * 0.5f,
        compA->scale.y * 0.5f,
        compA->scale.z * 0.5f,
    };
    
    float x = glm_max(compA->pos.x - halfA.x, glm_min(compB->pos.x, compA->pos.x + halfA.x));
    float y = glm_max(compA->pos.y - halfA.y, glm_min(compB->pos.y, compA->pos.y + halfA.y));
    float z = glm_max(compA->pos.z - halfA.z, glm_min(compB->pos.z, compA->pos.z + halfA.x));
    
    float distance = 
        (x - compB->pos.x)*(x - compB->pos.x)+
        (y - compB->pos.y)*(y - compB->pos.y)+
        (z - compB->pos.z)*(z - compB->pos.z);

    return distance < compB->radius * compB->radius;
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