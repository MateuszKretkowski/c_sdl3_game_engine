#ifndef SAMPLE_COMPONENT_H
#define SAMPLE_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} sample_component;

sample_component *create_sample_component();

#endif
