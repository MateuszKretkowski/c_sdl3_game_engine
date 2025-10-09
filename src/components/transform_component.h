#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
} TransformComponent;

TransformComponent *create_transform_component(Vector3 pos, Vector3 rot, Vector3 scale);

#endif