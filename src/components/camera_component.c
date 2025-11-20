#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camera_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void camera_awake(Component* self) {
    camera_component *cam = (camera_component*)self;
}

void camera_start(Component* self) {
    camera_component *cam = (camera_component*)self;
}

void camera_update(Component* self) {
    camera_component *cam = (camera_component*)self;
    transform_component *transform = get_component(self->gameObject, transform_component, "transform_component");

    if (!transform) {
        fprintf(stderr, "camera_update: Camera has no transform_component\n");
        return;
    }

    if (!cam->target) {
        // Only log once per frame to avoid spam
        static bool logged = false;
        if (!logged) {
            fprintf(stderr, "camera_update: Camera has no target (this message will only show once)\n");
            logged = true;
        }
        // Use default forward-looking camera if no target
        vec3 position = {transform->position.x, transform->position.y, transform->position.z};
        vec3 forward = {0.0f, 0.0f, -1.0f};
        vec3 target_pos;
        glm_vec3_add(position, forward, target_pos);
        vec3 up = {cam->up.x, cam->up.y, cam->up.z};
        glm_lookat(position, target_pos, up, cam->view);
    } else {
        vec3 position = {transform->position.x, transform->position.y, transform->position.z};
        vec3 target_pos = {cam->target->position.x, cam->target->position.y, cam->target->position.z};
        vec3 up = {cam->up.x, cam->up.y, cam->up.z};
        glm_lookat(position, target_pos, up, cam->view);
    }

    if (cam->is_orthographic) {
        float half_width = 10.0f;
        float half_height = half_width / cam->aspect_ratio;
        glm_ortho(-half_width, half_width, -half_height, half_height, cam->near_plane, cam->far_plane, cam->projection);
    } else {
        glm_perspective(glm_rad(cam->fov), cam->aspect_ratio, cam->near_plane, cam->far_plane, cam->projection);
    }
}

void camera_destroy(Component* self) {
    camera_component *cam = (camera_component*)self;
    if (cam->target_id) {
        free(cam->target_id);
        cam->target_id = NULL;
    }
}

camera_component *create_camera_component(transform_component *target, Vector3 up, float fov, float aspect_ratio, float near_plane, float far_plane, bool is_orthographic) {
    camera_component* cam = malloc(sizeof(camera_component));
    cam->base.id = strdup("camera_component");
    cam->base.name = strdup("Camera");
    cam->base.isActive = true;
    cam->base.gameObject = NULL;
    cam->base.size = sizeof(camera_component);
    cam->base.standard_voids = malloc(sizeof(component_standard_voids));
    cam->base.standard_voids->awake = camera_awake;
    cam->base.standard_voids->start = camera_start;
    cam->base.standard_voids->update = camera_update;
    cam->base.standard_voids->destroy = camera_destroy;

    glm_mat4_identity(cam->view);
    glm_mat4_identity(cam->projection);

    cam->target = target;
    cam->target_id = NULL;
    cam->up = up;
    cam->fov = fov;
    cam->aspect_ratio = aspect_ratio;
    cam->near_plane = near_plane;
    cam->far_plane = far_plane;
    cam->is_orthographic = is_orthographic;

    return cam;
}

Component* camera_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: camera_from_json received NULL json\n");
        return NULL;
    }

    transform_component *target = NULL;
    Vector3 up = {0.0f, 1.0f, 0.0f};
    float fov = 45.0f;
    float aspect_ratio = 16.0f / 9.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;
    bool is_orthographic = false;

    char *target_id_str = NULL;
    cJSON *target_json = cJSON_GetObjectItemCaseSensitive(json, "target");
    if (target_json) {
        if (target_json->valuestring) {
            target_id_str = strdup(target_json->valuestring);
            fprintf(stderr, "camera_from_json: Storing target_id '%s' for later resolution\n", target_id_str);
        }
        else if (cJSON_IsArray(target_json)) {
            Vector3 rot = {0, 0, 0};
            Vector3 scale = {1, 1, 1};
            transform_component *tc = create_transform_component(parse_vector3_array(target_json), rot, scale);
            target = tc;
        }
    }
    else {
      Vector3 origin = {0, 0, 0};
      Vector3 rot = {0, 0, 0};            // Store the target ID string to resolve later

      Vector3 scale = {1, 1, 1};
      target = create_transform_component(origin, rot, scale);
    }

    cJSON *up_json = cJSON_GetObjectItemCaseSensitive(json, "up");
    if (up_json) {
        up = parse_vector3_array(up_json);
    }

    cJSON *fov_json = cJSON_GetObjectItemCaseSensitive(json, "fov");
    if (fov_json && cJSON_IsNumber(fov_json)) {
        fov = (float)fov_json->valuedouble;
    }

    cJSON *aspect_json = cJSON_GetObjectItemCaseSensitive(json, "aspect_ratio");
    if (aspect_json && cJSON_IsNumber(aspect_json)) {
        aspect_ratio = (float)aspect_json->valuedouble;
    }

    cJSON *near_json = cJSON_GetObjectItemCaseSensitive(json, "near_plane");
    if (near_json && cJSON_IsNumber(near_json)) {
        near_plane = (float)near_json->valuedouble;
    }

    cJSON *far_json = cJSON_GetObjectItemCaseSensitive(json, "far_plane");
    if (far_json && cJSON_IsNumber(far_json)) {
        far_plane = (float)far_json->valuedouble;
    }

    cJSON *ortho_json = cJSON_GetObjectItemCaseSensitive(json, "is_orthographic");
    if (ortho_json && cJSON_IsBool(ortho_json)) {
        is_orthographic = cJSON_IsTrue(ortho_json);
    }

    camera_component *cam = create_camera_component(target, up, fov, aspect_ratio, near_plane, far_plane, is_orthographic);
    cam->target_id = target_id_str;
    return (Component*)cam;
}

void camera_get_view_matrix(camera_component *cam, mat4 out) {
    if (!cam) return;
    glm_mat4_copy(cam->view, out);
}

void camera_get_projection_matrix(camera_component *cam, mat4 out) {
    if (!cam) return;
    glm_mat4_copy(cam->projection, out);
}

void camera_set_perspective(camera_component *cam, float fov, float aspect, float near, float far) {
    if (!cam) return;
    cam->fov = fov;
    cam->aspect_ratio = aspect;
    cam->near_plane = near;
    cam->far_plane = far;
    cam->is_orthographic = false;
}

void camera_set_orthographic(camera_component *cam, float left, float right, float bottom, float top, float near, float far) {
    if (!cam) return;
    cam->near_plane = near;
    cam->far_plane = far;
    cam->is_orthographic = true;
    glm_ortho(left, right, bottom, top, near, far, cam->projection);
}

__attribute__((constructor))
static void register_camera_component() {
    component_registry_register("camera_component", camera_from_json);
}
