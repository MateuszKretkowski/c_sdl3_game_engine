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
        physics_m.gravity = parse_vector3_array(gravity);
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
        realloc(physics_m.gameObjects, sizeof(GameObject*)*physics_m.gameObjects_capacity);
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

void physics_manager_update() {
    spatial_system_update(physics_m.gameObjects);
}