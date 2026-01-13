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

void transform_translate(transform_component *t, Vector3 pos);
void transform_rotate(transform_component *t, Vector3 rot);

// Setters
void transform_set_position(transform_component *t, Vector3 pos);
void transform_set_rotation(transform_component *t, Vector3 rot);
void transform_set_scale(transform_component *t, Vector3 scale);

// Getters
Vector3 transform_get_position(transform_component *t);
Vector3 transform_get_rotation(transform_component *t);
Vector3 transform_get_scale(transform_component *t);

#endif