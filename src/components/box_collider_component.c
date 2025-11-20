#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "box_collider_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void box_collider_awake(Component* self) {
    box_collider_component *comp = (box_collider_component*)self;
}

void box_collider_start(Component* self) {
    box_collider_component *comp = (box_collider_component*)self;
}

void box_collider_update(Component* self) {
    box_collider_component *comp = (box_collider_component*)self;
}

void box_collider_destroy(Component* self) {
    box_collider_component *comp = (box_collider_component*)self;
}

bool is_point_inside_AABB(box_collider_component *comp, Vector3 pos) {
    Vector3 half = {
        comp->scale.x * 0.5f,
        comp->scale.y * 0.5f,
        comp->scale.z * 0.5f
    };

    return (
        pos.x >= comp->pos.x - half.x &&
        pos.x <= comp->pos.x + half.x  &&
        pos.y >= comp->pos.y - half.y  &&
        pos.y <= comp->pos.y + half.y  &&
        pos.z >= comp->pos.z - half.z  &&
        pos.z <= comp->pos.z + half.z 
    );
}

box_collider_component *create_box_collider_component(Vector3 pos, Vector3 scale) {
    box_collider_component* comp = malloc(sizeof(box_collider_component));
    comp->base.id = strdup("box_collider_component");
    comp->base.name = strdup("box_collider");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(box_collider_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = box_collider_awake;
    comp->base.standard_voids->start = box_collider_start;
    comp->base.standard_voids->update = box_collider_update;
    comp->base.standard_voids->destroy = box_collider_destroy;

    comp->pos = pos;
    comp->scale = scale;

    return comp;
}

Component* box_collider_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: box_collider_from_json received NULL json\n");
        return NULL;
    }

    return (Component*)create_box_collider_component();
}

__attribute__((constructor))
static void register_box_collider_component() {
    component_registry_register("sample_component", box_collider_from_json);
}
