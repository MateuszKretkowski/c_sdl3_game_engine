#ifndef SPATIAL_SYSTEM_H
#define SPATIAL_SYSTEM_H

#include "utils/utils.h"
#include "utils/collision_utils.h"
#include "core/gameObject.h"
#include "scene_manager.h"
#include "components/components.h"
#include "json_utils.h"
#include "math.h"

typedef struct {
    float cell_size;
    int grid_width;
    int grid_height;
    int grid_depth;
    int grid_cell_capacity;
} spatial_system_config;

typedef struct {
    GameObject *objects;
    int count;
    int capacity;
} grid_cell;

void spatial_system_init(GameObject *objects, int gameObject_length);
void spatial_system_update(GameObject *gameObjects);

#endif