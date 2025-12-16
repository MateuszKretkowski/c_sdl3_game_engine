#include "physics_manager.h"

physics_manager *physics_m = NULL;

void physics_manager_init(cJSON *json) {
    cJSON *physics_config = cJSON_GetObjectItemCaseSensitive(json, "physics");
    if (!physics_config) {
        printf("physics_manager_init(): physics config not found.\n");
        return;
    }

    physics_m = malloc(sizeof(physics_manager));
    
    cJSON *gravity = cJSON_GetObjectItemCaseSensitive(physics_config, "gravity");
    if (gravity && cJSON_IsArray(gravity)) {
        physics_m->gravity_force = parse_vector3_array(gravity);
    }
    
    cJSON *timestep = cJSON_GetObjectItemCaseSensitive(physics_config, "timestep");
    if (timestep && timestep->valuedouble) {
        physics_m->timestep = timestep->valuedouble;
    }
        
    cJSON *air_resistance = cJSON_GetObjectItemCaseSensitive(physics_config, "air_resistance");
    if (air_resistance && air_resistance->valuedouble) {
        physics_m->air_resistance = air_resistance->valuedouble;
    }

    cJSON *max_velocity = cJSON_GetObjectItemCaseSensitive(physics_config, "max_velocity");
    if (max_velocity && max_velocity->valuedouble) {
        physics_m->max_velocity = max_velocity->valuedouble;
    }

    physics_m->gameObjects = malloc(sizeof(GameObject*) * 1024);
    physics_m->gameObjects_length = 0;
    physics_m->gameObjects_capacity = 1024;

    spatial_system_init(physics_m->gameObjects, physics_m->gameObjects_length);
}

void physics_manager_add(GameObject *gameObject) {
    if (!gameObject || !gameObject->id || !get_component(gameObject, rigid_body_component, "rigid_body_component")) {
        return;
    }
    if (physics_m->gameObjects_length == physics_m->gameObjects_capacity) {
        physics_m->gameObjects_capacity *= 2;
        physics_m->gameObjects = realloc(physics_m->gameObjects, sizeof(GameObject*)*physics_m->gameObjects_capacity);
    }
    physics_m->gameObjects[physics_m->gameObjects_length] = gameObject;
    physics_m->gameObjects_length++;
}

void physics_manager_remove(GameObject *gameObject) {
    if (!gameObject || !gameObject->id || !get_component(gameObject, rigid_body_component, "rigid_body_component")) {
        return;
    }
    if (physics_m->gameObjects_length == 0) {
        return;
    }
    for (int i=0; i<physics_m->gameObjects_length; i++) {
        if (physics_m->gameObjects[i] == gameObject) {
            physics_m->gameObjects[i] = physics_m->gameObjects[physics_m->gameObjects_length - 1];
            physics_m->gameObjects_length--;
            break;
        }
    }
}

void physics_manager_handle_collision(GameObject *objA, GameObject *objB, Vector3 normal, float depth) {
    rigid_body_component *rb_a = get_component(objA, rigid_body_component, "rigid_body_component");
    rigid_body_component *rb_b = get_component(objB, rigid_body_component, "rigid_body_component");

    if (rb_a->is_kinematic && rb_b->is_kinematic) {
        return;
    }

    transform_component *transform_a = get_component(objA, transform_component, "transform_component");
    transform_component *transform_b = get_component(objB, transform_component, "transform_component");

    // calculate depth
    printf("depth: %f\n", depth);
    if (!rb_a->is_kinematic && depth != 0) {
        transform_a->position = vector3_subtract(transform_a->position, vector3_multiply(normal, -depth/2));
    }
    if (!rb_b->is_kinematic && depth != 0) {
        transform_b->position = vector3_subtract(transform_b->position, vector3_multiply(normal, depth/2));
    }
    
    physics_material *pm_a = rb_a->physics_material;
    physics_material *pm_b = rb_b->physics_material;

    Vector3 Ua = rb_a->velocity;
    Vector3 Ub = rb_b->velocity;
    float ma = rb_a->mass;
    float mb = rb_b->mass;

    Vector4 color;
    color.x = 1;
    color.y = 1;
    color.z = 1;
    color.w = 1;
    // vector3_divide(vector3_add(transform_a->position, transform_b->position), 2)
    
    // SPHERE_A
    // velocity before collision is length, times the orthogonal unit (1) vector. + length times the parallel unit vector
    
    // Uao = (Pb - Pa)/length(Pb - Pa)
    // calculating Uao:

    Vector3 Uao = normal;

    
    // calculating Uap:
    Vector3 Uap = vector3_zero();
    Vector3 cross_ua_uao = vector3_cross(Ua, Uao);
    if (vector3_length(cross_ua_uao) > 0.0001f) {
        Vector3 cross_result = vector3_cross(cross_ua_uao, Uao);
        if (vector3_length(cross_result) > 0.0001f) {
            Uap = vector3_divide(cross_result, vector3_length(cross_result));
        }
    }
    // debug_draw_sphere(Uao, 0.2, color, 0.01);
    
    // Lao = dot(Uao, Ua)
    float Lao = vector3_dot(Uao, Ua);
    float Lap = vector3_dot(Uap, Ua);

    float Lap_friction_force = rb_a->physics_material->friction * Lap;
    
    if (fabs(Lap_friction_force) >= fabs(Lap)) {
        Lap = 0;
    }
    else {
        Lap -= Lap_friction_force;
    }
    
    // SPHERE_B
    // same here, but instead of Ubo we take Uao, because if we took Ubo, then the vector would had opposite direction.
    
    // so we don't calculkate Ubo, we take Uao.
    
    Vector3 Ubp = vector3_zero();
    Vector3 cross_ub_uao = vector3_cross(Ub, Uao);
    if (vector3_length(cross_ub_uao) > 0.0001f) {
        Vector3 cross_result_b = vector3_cross(cross_ub_uao, Uao);
        if (vector3_length(cross_result_b) > 0.0001f) {
            Ubp = vector3_divide(cross_result_b, vector3_length(cross_result_b));
        }
    }
    
    float Lbo = vector3_dot(Uao, Ub);
    float Lbp = vector3_dot(Ubp, Ub);

    float Lbp_friction_force = rb_b->physics_material->friction * Lbp;

    if (fabs(Lbp_friction_force) >= fabs(Lbp)) {
        Lbp = 0;
    }
    else {
        Lbp -= Lbp_friction_force;
    }

    
    printf("GameObject: %s, Uao.y: %f\n", rb_b->base.gameObject->id, Uao.y);
    printf("GameObject: %s, Lao: %f\n", rb_b->base.gameObject->id, Lbo);
    
    // now we have defined every variable that we need to calculate new velocities:

    float avg_restitution = (pm_a->restitution + pm_b->restitution)/2;

    if (!rb_a->is_kinematic) {
        Vector3 Va = vector3_add(vector3_multiply(Uao, ((Lao * ma + Lbo * mb + (Lbo - Lao) * mb * avg_restitution) / (ma + mb))), vector3_multiply(Uap, Lap));
        rb_a->velocity = Va;
    } 
    if (!rb_b->is_kinematic) {
        Vector3 Vb = vector3_add(vector3_multiply(Uao, ((Lao * ma + Lbo * mb + (Lao - Lbo) * ma * avg_restitution) / (ma + mb))), vector3_multiply(Ubp, Lbp));
        rb_b->velocity = Vb;
    }
}

void physics_manager_calculate_objects() {
    for (int i=0; i<physics_m->gameObjects_length; i++) {
        GameObject *curr = physics_m->gameObjects[i];
        rigid_body_component *rb =  get_component(curr, rigid_body_component, "rigid_body_component");
        transform_component *transform = get_component(curr, transform_component, "transform_component");
        if (rb->use_gravity == true) {
            rb->acceleration = vector3_add(rb->acceleration, physics_m->gravity_force);
        }
        Vector3 currVelocity = vector3_add(rb->velocity, vector3_multiply(rb->acceleration, physics_m->timestep));
        if (vector3_length(currVelocity) > physics_m->max_velocity) {
            currVelocity = vector3_multiply(vector3_normalize(currVelocity), physics_m->max_velocity);
        }
        currVelocity = vector3_multiply(currVelocity, physics_m->air_resistance);
        rb->velocity = currVelocity;
        Vector3 currPos = vector3_add(transform->position, vector3_multiply(currVelocity, physics_m->timestep));
        transform->position = currPos;
        rb->acceleration = vector3_zero();
    }
}

void physics_manager_update() {
    physics_manager_calculate_objects();
    spatial_system_update(physics_m->gameObjects, physics_m->gameObjects_length);
}