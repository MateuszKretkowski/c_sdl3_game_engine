#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "utils/utils.h"
#include "utils/collision_utils.h"
#include "core/gameObject.h"
#include "components/components.h"
#include "scene_manager.h"
#include "spatial_system.h"

typedef struct {
    GameObject **gameObjects;
    int gameObjects_length;
    int gameObjects_capacity;

    Vector3 gravity_force;
    float timestep;
    float air_resistance;
    float max_velocity;
} physics_manager;

extern physics_manager *physics_m;

void physics_manager_init(cJSON *json);
void physics_manager_add(GameObject *gameObject);
void physics_manager_remove(GameObject *gameObject);
void physics_manager_update();

void physics_manager_handle_collision(GameObject *objA, GameObject *objB, Vector3 normal, float depth);

#endif