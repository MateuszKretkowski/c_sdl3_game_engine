#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rigid_body_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void rigid_body_awake(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

void rigid_body_start(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

void rigid_body_update(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

void rigid_body_destroy(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

rigid_body_component *create_rigid_body_component() {
    rigid_body_component* comp = malloc(sizeof(rigid_body_component));
    comp->base.id = strdup("rigid_body_component");
    comp->base.name = strdup("RigidBody");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(rigid_body_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = rigid_body_awake;
    comp->base.standard_voids->start = rigid_body_start;
    comp->base.standard_voids->update = rigid_body_update;
    comp->base.standard_voids->destroy = rigid_body_destroy;

    return comp;
}

Component* rigid_body_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: rigid_body_from_json received NULL json\n");
        return NULL;
    }

    return (Component*)create_rigid_body_component();
}

__attribute__((constructor))
static void register_rigid_body_component() {
    component_registry_register("rigid_body_component", rigid_body_from_json);
}
