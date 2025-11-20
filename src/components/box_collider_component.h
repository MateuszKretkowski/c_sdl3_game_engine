#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    Vector3 pos;
    Vector3 scale;
} box_collider_component;

bool is_point_inside_AABB(box_collider_component *comp, Vector3 pos);

box_collider_component *create_box_collider_component();

#endif
