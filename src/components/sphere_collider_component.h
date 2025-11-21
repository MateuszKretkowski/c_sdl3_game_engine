#ifndef SPHERE_COLLIDER_COMPONENT_H
#define SPHERE_COLLIDER_COMPONENT_H

#include "utils.h"
#include "core/component.h"
#include "components.h"

typedef struct {
    Component base;

    Vector3 pos;
    float radius;
} sphere_collider_component;

bool is_point_inside_sphere(sphere_collider_component *comp, Vector3 point);
bool intersect_sphere_sphere(sphere_collider_component *compA, sphere_collider_component *compB);
bool intersect_AABB_sphere(box_collider_component *compA, sphere_collider_component *compB);

sphere_collider_component *create_sphere_collider_component();

#endif
