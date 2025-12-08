#ifndef SAMPLE_COMPONENT_H
#define SAMPLE_COMPONENT_H

#include "utils.h"
#include "core/component.h"
#include "physics/physics_utils.h"

typedef struct {
    Component base;

} sample_component;

sample_component *create_sample_component();

#endif
