#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "utils/utils.h"
#include "utils/collision_utils.h"
#include "core/gameObject.h"
#include "components.h"
#include "scene_manager.h"
#include "spatial_system.h"

typedef struct {
    GameObject *gameObjects;
    int gameObjects_length;
    int gameObjects_capacity;

    Vector3 gravity;
    float timestep;
    float max_velocity;
} physics_manager;

#endif