#ifndef SPRING_COMPONENT_H
#define SPRING_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    float stiffness;

    GameObject *obj_1;
    GameObject *obj_2;
} spring_component;

spring_component *create_spring_component(float stiffness, GameObject *obj_1, GameObject *obj_2);

#endif
