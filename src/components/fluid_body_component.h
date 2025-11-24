#ifndef FLUID_BODY_COMPONENT
#define FLUID_BODY_COMPONENT

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} fluid_body_component;

fluid_body_component *create_fluid_body_component();

#endif
