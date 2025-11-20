#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fluid_body_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void fluid_body_awake(Component* self) {
    fluid_body_component *comp = (fluid_body_component*)self;
}

void fluid_body_start(Component* self) {
    fluid_body_component *comp = (fluid_body_component*)self;
}

void fluid_body_update(Component* self) {
    fluid_body_component *comp = (fluid_body_component*)self;
}

void fluid_body_destroy(Component* self) {
    fluid_body_component *comp = (fluid_body_component*)self;
}

fluid_body_component *create_fluid_body_component() {
    fluid_body_component* comp = malloc(sizeof(fluid_body_component));
    comp->base.id = strdup("fluid_body_component");
    comp->base.name = strdup("FluidBody");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(fluid_body_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = fluid_body_awake;
    comp->base.standard_voids->start = fluid_body_start;
    comp->base.standard_voids->update = fluid_body_update;
    comp->base.standard_voids->destroy = fluid_body_destroy;

    return comp;
}

Component* fluid_body_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: fluid_body_from_json received NULL json\n");
        return NULL;
    }

    return (Component*)create_fluid_body_component();
}

__attribute__((constructor))
static void register_fluid_body_component() {
    component_registry_register("fluid_body_component", fluid_body_from_json);
}
