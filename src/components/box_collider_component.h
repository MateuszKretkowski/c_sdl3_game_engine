#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    Vector3 pos;
    Vector3 scale;
} box_collider_component;

bool is_point_inside_box_collider(box_collider_component *comp, Vector3 pos);
bool intersect_AABB_AABB(box_collider_component *compA, box_collider_component *compB);

box_collider_component *create_box_collider_component();

#endif
