#ifndef PLATFORM_CONTROLLER_COMPONENT_H
#define PLATFORM_CONTROLLER_COMPONENT_H

#include "utils.h"
#include "core/component.h"
#include "rigid_body_component.h"
#include "transform_component.h"

typedef struct {
    Component base;

    float stiffness;
    float length;

    rigid_body_component *obj_1;
    transform_component *tc_1;
    rigid_body_component *obj_2;
    transform_component *tc_2;
} platform_controller_component;

platform_controller_component *create_platform_controller_component();

#endif
