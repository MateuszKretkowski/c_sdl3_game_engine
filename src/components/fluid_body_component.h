#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} fluid_body_component;

fluid_body_component *create_fluid_body_component();

#endif
