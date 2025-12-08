#ifndef RIGID_BODY_COMPONENT
#define RIGID_BODY_COMPONENT

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    physics_material *physics_material;

    float mass;
    Vector3 velocity;
    Vector3 acceleration;
} rigid_body_component;

rigid_body_component *create_rigid_body_component(physics_material *pmc);

#endif
