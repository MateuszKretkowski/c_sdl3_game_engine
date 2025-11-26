#ifndef SPATIAL_SYSTEM_H
#define SPATIAL_SYSTEM_H

#include "utils/utils.h"
#include "utils/collision_utils.h"
#include "core/gameObject.h"
#include "components.h"
#include "json_utils.h"

typedef struct {
    float cell_size;
    int grid_width;
    int grid_height;
    int grid_depth;
} spatial_system_config;

typedef struct {
    GameObject *objects;
    int count;
    int capacity;
} grid_cell;

#endif