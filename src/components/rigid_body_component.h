#ifndef RIGID_BODY_COMPONENT
#define RIGID_BODY_COMPONENT

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} rigid_body_component;

rigid_body_component *create_rigid_body_component();

#endif
