#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transform_component.h"
#include "component_registry.h"
#include "json_utils.h"

#include <cglm/cglm.h>

void transform_awake(Component* self) {
    TransformComponent *t = (TransformComponent*)self;
}

void transform_start(Component* self) {
    TransformComponent *t = (TransformComponent*)self;
}

void transform_update(Component* self) {
    TransformComponent *t = (TransformComponent*)self;
    glm_mat4_identity(t->model);
    
    vec3 pos = {t->position.x, t->position.y, t->position.z};
    glm_translate(t->model, pos);

    vec3 x_axis = {1.0f, 0.0f, 0.0f};
    vec3 y_axis = {0.0f, 1.0f, 0.0f};
    vec3 z_axis = {0.0f, 0.0f, 1.0f};
    glm_rotate(t->model, glm_rad(t->rotation.x), x_axis);
    glm_rotate(t->model, glm_rad(t->rotation.y), y_axis);
    glm_rotate(t->model, glm_rad(t->rotation.z), z_axis);

    vec3 scale = {t->scale.x, t->scale.y, t->scale.z};
    glm_scale(t->model, scale);
}

void transform_destroy(Component* self) {
    TransformComponent *t = (TransformComponent*)self;

}

TransformComponent *create_transform_component(Vector3 pos, Vector3 rot, Vector3 scale) {
    TransformComponent* t = malloc(sizeof(TransformComponent));
    t->base.id = "transform_component";
    t->base.name = "Transform";
    t->base.isActive = true;
    t->base.gameObject = NULL;
    t->base.standard_voids = malloc(sizeof(component_standard_voids));
    t->base.standard_voids->awake = transform_awake;
    t->base.standard_voids->start = transform_start;
    t->base.standard_voids->update = transform_update;
    t->base.standard_voids->destroy = transform_destroy;

    // variables:
    t->position = pos;
    t->rotation = rot;
    t->scale = scale;
    glm_mat4_identity(t->model);

    return t;
}

Component* transform_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: transform_from_json received NULL json\n");
        return NULL;
    }

    Vector3 position = {0, 0, 0};
    Vector3 rotation = {0, 0, 0};
    Vector3 scale = {1, 1, 1};

    cJSON *pos_json = cJSON_GetObjectItemCaseSensitive(json, "position");
    if (pos_json) {
        position = parse_vector3_array(pos_json);
    }

    cJSON *rot_json = cJSON_GetObjectItemCaseSensitive(json, "rotation");
    if (rot_json) {
        rotation = parse_vector3_array(rot_json);
    }

    cJSON *scale_json = cJSON_GetObjectItemCaseSensitive(json, "scale");
    if (scale_json) {
        scale = parse_vector3_array(scale_json);
    }

    return (Component*)create_transform_component(position, rotation, scale);
}

__attribute__((constructor))
static void register_transform_component() {
    component_registry_register("transform_component", transform_from_json);
}