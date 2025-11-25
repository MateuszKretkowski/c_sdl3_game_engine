#ifndef RIGID_BODY_COMPONENT
#define RIGID_BODY_COMPONENT

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    float mass;
    float friction;
    bool use_gravity;
    Vector3 resultant_force;
} rigid_body_component;

rigid_body_component *create_rigid_body_component();

#endif
