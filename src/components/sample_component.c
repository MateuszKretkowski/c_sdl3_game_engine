#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sample_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void sample_awake(Component* self) {
    sample_component *comp = (sample_component*)self;
}

void sample_start(Component* self) {
    sample_component *comp = (sample_component*)self;
    
    if (strcmp(self->gameObject->id, "sphere_1") == 0) {
        Vector3 direction = {1, 0, 0};
        float force = 75;
        add_force(get_component(self->gameObject, rigid_body_component, "rigid_body_component"), direction, force);
    }
    if (strcmp(self->gameObject->id, "sphere_2") == 0) {
        Vector3 direction = {-1, 0, 0};
        float force = 75;
        add_force(get_component(self->gameObject, rigid_body_component, "rigid_body_component"), direction, force);
    }
}

void sample_update(Component* self) {
    sample_component *comp = (sample_component*)self;
    transform_component *t = get_component(self->gameObject, transform_component, "transform_component");
    if (!t) {
        fprintf(stderr, "no transform_component on %s gameObject\n", self->gameObject->name);
        return;
    }
}

void sample_destroy(Component* self) {
    sample_component *comp = (sample_component*)self;
}

sample_component *create_sample_component() {
    sample_component* comp = malloc(sizeof(sample_component));
    comp->base.id = strdup("sample_component");
    comp->base.name = strdup("Sample");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(sample_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = sample_awake;
    comp->base.standard_voids->start = sample_start;
    comp->base.standard_voids->update = sample_update;
    comp->base.standard_voids->destroy = sample_destroy;

    return comp;
}

Component* sample_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: sample_from_json received NULL json\n");
        return NULL;
    }

    return (Component*)create_sample_component();
}

__attribute__((constructor))
static void register_sample_component() {
    component_registry_register("sample_component", sample_from_json);
}