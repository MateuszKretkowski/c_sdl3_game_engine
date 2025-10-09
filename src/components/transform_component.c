#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transform_component.h"
#include "component_registry.h"
#include "json_utils.h"

void transform_awake(Component* self) {
    TransformComponent *t = (TransformComponent*)self;
}

void transform_start(Component* self) {
    TransformComponent *t = (TransformComponent*)self;
}

void transform_update(Component* self) {
    TransformComponent *t = (TransformComponent*)self;
    
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