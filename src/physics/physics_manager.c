#include "physics_manager.h"

physics_manager physics_m;

void physics_manager_init(cJSON *json) {
    cJSON *physics_config = cJSON_GetObjectItemCaseSensitive(json, "physics");
    if (!physics_config) {
        printf("physics_manager_init(): physics config not found.\n");
        return;
    }
    
    cJSON *gravity = cJSON_GetObjectItemCaseSensitive(physics_config, "gravity");
    if (gravity && cJSON_IsArray(gravity)) {
        physics_m.gravity_force = parse_vector3_array(gravity);
    }
    
    cJSON *timestep = cJSON_GetObjectItemCaseSensitive(physics_config, "timestep");
    if (timestep && timestep->valuedouble) {
        physics_m.timestep = timestep->valuedouble;
    }
    
    cJSON *max_velocity = cJSON_GetObjectItemCaseSensitive(physics_config, "max_velocity");
    if (max_velocity && max_velocity->valuedouble) {
        physics_m.max_velocity = max_velocity->valuedouble;
    }

    physics_m.gameObjects = malloc(sizeof(GameObject*) * 1024);
    physics_m.gameObjects_length = 0;
    physics_m.gameObjects_capacity = 1024;
}

void physics_manager_add(GameObject *gameObject) {
    if (!gameObject || !gameObject->id || !get_component(gameObject, rigid_body_component, "rigid_body_component")) {
        return;
    }
    if (physics_m.gameObjects_length == physics_m.gameObjects_capacity) {
        physics_m.gameObjects_capacity *= 2;
        physics_m.gameObjects = realloc(physics_m.gameObjects, sizeof(GameObject*)*physics_m.gameObjects_capacity);
    }
    physics_m.gameObjects[physics_m.gameObjects_length] = *gameObject;
    physics_m.gameObjects_length++;
}

void physics_manager_remove(GameObject *gameObject) {
    if (!gameObject || !gameObject->id || !get_component(gameObject, rigid_body_component, "rigid_body_component")) {
        return;
    }
    if (physics_m.gameObjects_length == 0) {
        return;
    }
    for (int i=0; i<physics_m.gameObjects_length; i++) {
        if (&physics_m.gameObjects[i] == gameObject) {
            physics_m.gameObjects[i] = physics_m.gameObjects[physics_m.gameObjects_length];
            physics_m.gameObjects_length--;
        }
    }
}

void physics_manager_handle_collision(GameObject *objA, GameObject *objB) {
    rigid_body_component *rb_a = get_component(objA, rigid_body_component, "rigid_body_component");
    rigid_body_component *rb_b = get_component(objB, rigid_body_component, "rigid_body_component");


}

void calculate_inelastic_collision(float mass_a, float speed_a, float mass_b, float speed_b) {
    float momentum = (mass_a*speed_a)-(mass_b*speed_b);

}

void physics_manager_calculate_objects() {
    for (int i=0; i<physics_m.gameObjects_length; i++) {
        GameObject *curr = &physics_m.gameObjects[i];
        rigid_body_component *rb =  get_component(curr, rigid_body_component, "rigid_body_component");
        transform_component *transform = get_component(curr, transform_component, "transform_component");
        rb->acceleration = vector3_add(rb->acceleration, physics_m.gravity_force);
        Vector3 currVelocity = vector3_add(rb->velocity, vector3_multiply(rb->acceleration, physics_m.timestep));
        if (vector3_length(currVelocity) > physics_m.max_velocity) {
            currVelocity = vector3_multiply(vector3_normalize(currVelocity), physics_m.max_velocity);
        }
        rb->velocity = currVelocity;
        Vector3 currPos = vector3_add(transform->position, vector3_multiply(currVelocity, physics_m.timestep));
        transform->position = currPos;
        rb->acceleration = vector3_zero();
    }
}

void physics_manager_update() {
    physics_manager_calculate_objects();
    spatial_system_update(physics_m.gameObjects);
}