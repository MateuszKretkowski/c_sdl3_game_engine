#ifndef SOFT_BODY_COMPONENT
#define SOFT_BODY_COMPONENT

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    physics_material *physics_material;
    float mass;
    bool use_gravity;
    bool is_kinematic;
    int child_count;

    Vector3 velocity;
    GameObject **gameObjects;
} soft_body_component;

soft_body_component *create_soft_body_component(physics_material *pm, float mass, bool use_gravity, bool is_kinematic, int child_count);

#endif
