#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    Vector3 pos;
    Vector3 scale;
} box_collider_component;

box_collider_component *create_box_collider_component(Vector3 pos, Vector3 scale);

#endif
