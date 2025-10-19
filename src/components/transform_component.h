#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "utils.h"
#include "core/component.h"
#include <cglm/cglm.h>

typedef struct {
    Component base;

    mat4 model;

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
} transform_component;

transform_component *create_transform_component(Vector3 pos, Vector3 rot, Vector3 scale);

#endif