#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    Vector3 pos;
    float radius;
} sphere_collider_component;

sphere_collider_component *create_sphere_collider_component();

#endif
