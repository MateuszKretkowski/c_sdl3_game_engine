#ifndef COLLISION_UTILS_H
#define COLLISION_UTILS_H

#include "stdio.h"
#include "stdlib.h"
#include "components/sphere_collider_component.h"
#include "components/box_collider_component.h"

bool intersect_point_AABB(box_collider_component *comp, Vector3 pos);
bool intersect_AABB_AABB(box_collider_component *compA, box_collider_component *compB);

bool intersect_point_sphere(sphere_collider_component *comp, Vector3 point);
bool intersect_sphere_sphere(sphere_collider_component *compA, sphere_collider_component *compB);
bool intersect_AABB_sphere(box_collider_component *compA, sphere_collider_component *compB); 

#endif