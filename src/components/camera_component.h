#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "utils.h"
#include "core/component.h"
#include <cglm/cglm.h>
#include "transform_component.h"

typedef struct {
    Component base;

    mat4 view;
    mat4 projection;

    transform_component *target;
    char *target_id;  // ID of GameObject to target (resolved after scene load)
    Vector3 up;

    float fov;
    float aspect_ratio;
    float near_plane;
    float far_plane;

    bool is_orthographic;
} camera_component;

camera_component *create_camera_component(transform_component *target, Vector3 up, float fov, float aspect_ratio, float near_plane, float far_plane, bool is_orthographic);

void camera_get_view_matrix(camera_component *cam, mat4 out);
void camera_get_projection_matrix(camera_component *cam, mat4 out);
void camera_set_perspective(camera_component *cam, float fov, float aspect, float near, float far);
void camera_set_orthographic(camera_component *cam, float left, float right, float bottom, float top, float near, float far);

#endif
