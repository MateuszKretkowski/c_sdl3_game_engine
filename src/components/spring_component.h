#ifndef SPRING_COMPONENT_H
#define SPRING_COMPONENT_H

#include "utils.h"
#include "core/component.h"
#include "components/components.h"

typedef struct {
    Component base;

    float stiffness;
    float length;

    rigid_body_component *obj_1;
    transform_component *tc_1;
    rigid_body_component *obj_2;
    transform_component *tc_2;
} spring_component;

spring_component *create_spring_component(float stiffness, rigid_body_component *obj_1, transform_component *tc_1, rigid_body_component *obj_2, transform_component *tc_2);

#endif
