#include "collision_utils.h"

bool intersect_sphere_point(sphere_collider_component *comp, Vector3 point) {
    float distance = 
        (point.x - comp->pos.x)*(point.x - comp->pos.x) +
        (point.y - comp->pos.y)*(point.y - comp->pos.y) +
        (point.z - comp->pos.z)*(point.z - comp->pos.z);
        
    return distance < comp->radius * comp->radius;
}

Vector3 intersect_sphere_sphere(sphere_collider_component *compA, sphere_collider_component *compB, float *depth) {
    transform_component *transform_a = get_component(compA->base.gameObject, transform_component, "transform_component");
    transform_component *transform_b = get_component(compB->base.gameObject, transform_component, "transform_component");
    float distance = 
        (transform_a->position.x - transform_b->position.x)*(transform_a->position.x - transform_b->position.x)+
        (transform_a->position.y - transform_b->position.y)*(transform_a->position.y - transform_b->position.y)+
        (transform_a->position.z - transform_b->position.z)*(transform_a->position.z - transform_b->position.z);
    
    float radiusSum = compA->radius + compB->radius;
    if (distance <= radiusSum * radiusSum) {
        *depth = radiusSum - sqrt(distance);
        return vector3_divide(vector3_subtract(transform_a->position, transform_b->position), vector3_length(vector3_subtract(transform_a->position, transform_b->position))); 
    }
    else {
        return vector3_zero();
    }
}

Vector3 intersect_AABB_sphere(box_collider_component *compA, sphere_collider_component *compB, float *depth) {
    transform_component *transform_a = get_component(compA->base.gameObject, transform_component, "transform_component");
    transform_component *transform_b = get_component(compB->base.gameObject, transform_component, "transform_component");
    Vector3 halfA = {
        compA->scale.x * 0.5f,
        compA->scale.y * 0.5f,
        compA->scale.z * 0.5f,
    };

    Vector3 a_min = vector3_zero();
    a_min.x = transform_a->position.x - halfA.x;
    a_min.y = transform_a->position.y - halfA.y;
    a_min.z = transform_a->position.z - halfA.z;
    
    Vector3 a_max = vector3_zero();
    a_max.x = transform_a->position.x + halfA.x;
    a_max.y = transform_a->position.y + halfA.y;
    a_max.z = transform_a->position.z + halfA.z;
    
    Vector3 p_avg = vector3_zero();
    p_avg.x = glm_clamp(transform_b->position.x, a_min.x, a_max.x);
    p_avg.y = glm_clamp(transform_b->position.y, a_min.y, a_max.y);
    p_avg.z = glm_clamp(transform_b->position.z, a_min.z, a_max.z);

    float distance =
        (p_avg.x - transform_b->position.x)*(p_avg.x - transform_b->position.x)+
        (p_avg.y - transform_b->position.y)*(p_avg.y - transform_b->position.y)+
        (p_avg.z - transform_b->position.z)*(p_avg.z - transform_b->position.z);

    if (distance > compB->radius * compB->radius) {
        return vector3_zero();
    }
    else if (distance == compB->radius * compB->radius) {
        *depth = compB->radius - sqrt(distance);
        printf("distance == radius^2\n");
        return vector3_divide(vector3_subtract(transform_b->position, p_avg), vector3_length(vector3_subtract(transform_b->position, p_avg)));
    }
    else {
        float closest;
        float left = transform_b->position.x - a_min.x + compB->radius;
        float right = a_max.x - transform_b->position.x + compB->radius;
        float down = transform_b->position.y - a_min.y + compB->radius;
        float up = a_max.y - transform_b->position.y + compB->radius;
        float back = transform_b->position.z - a_min.z + compB->radius;
        float front = a_max.z - transform_b->position.z + compB->radius;
        float distances[6];
        distances[0] = left;
        distances[1] = right;
        distances[2] = up;
        distances[3] = down;
        distances[4] = front;
        distances[5] = back;
        closest = distances[0];
        for (int i=0; i<6; i++) {
            if (distances[i] < closest) {
                closest = distances[i];
            }
        }
        *depth = closest;
        if (closest == left) {
            printf("Collision direction: LEFT\n");
            return vector3_right();
        }
        else if (closest == right) {
            printf("Collision direction: RIGHT\n");
            return vector3_left();
        }
        else if (closest == down) {
            printf("Collision direction: DOWN\n");
            return vector3_up();
        }
        else if (closest == up) {
            printf("Collision direction: UP\n");
            return vector3_down();
        }
        else if (closest == back) {
            printf("Collision direction: BACK\n");
            return vector3_forward();
        }
        else {
            printf("Collision direction: FRONT\n");
            return vector3_back();
        }
    }
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

Vector3 intersect_AABB_AABB(box_collider_component *compA, box_collider_component *compB, float *depth) {
    transform_component *transform_a = get_component(compA->base.gameObject, transform_component, "transform_component");
    transform_component *transform_b = get_component(compB->base.gameObject, transform_component, "transform_component");

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

    float overlap_x = (halfA.x + halfB.x) - fabsf(transform_a->position.x - transform_b->position.x);
    float overlap_y = (halfA.y + halfB.y) - fabsf(transform_a->position.y - transform_b->position.y);
    float overlap_z = (halfA.z + halfB.z) - fabsf(transform_a->position.z - transform_b->position.z);

    if (overlap_x <= 0 || overlap_y <= 0 || overlap_z <= 0) {
        return vector3_zero();
    }

    float min_overlap = overlap_x;
    int axis = 0;

    if (overlap_y < min_overlap) {
        min_overlap = overlap_y;
        axis = 1;
    }
    if (overlap_z < min_overlap) {
        min_overlap = overlap_z;
        axis = 2;
    }

    *depth = min_overlap;

    if (axis == 0) {
        return (transform_a->position.x < transform_b->position.x) ? vector3_right() : vector3_left();
    } else if (axis == 1) {
        return (transform_a->position.y < transform_b->position.y) ? vector3_up() : vector3_down();
    } else {
        return (transform_a->position.z < transform_b->position.z) ? vector3_forward() : vector3_back();
    }
}