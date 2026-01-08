#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spring_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void spring_awake(Component* self) {
    spring_component *comp = (spring_component*)self;
}

void spring_start(Component* self) {
    spring_component *comp = (spring_component*)self;
}

void spring_update(Component* self) {
    spring_component *comp = (spring_component*)self;
}

void spring_destroy(Component* self) {
    spring_component *comp = (spring_component*)self;
}

spring_component *create_spring_component(float stiffness, GameObject *obj_1, GameObject *obj_2) {
    spring_component* comp = malloc(sizeof(spring_component));
    comp->base.id = strdup("spring_component");
    comp->base.name = strdup("spring");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(spring_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = spring_awake;
    comp->base.standard_voids->start = spring_start;
    comp->base.standard_voids->update = spring_update;
    comp->base.standard_voids->destroy = spring_destroy;

    comp->stiffness = stiffness;

    comp->obj_1 = obj_1;
    comp->obj_2 = obj_2;

    return comp;
}

Component* spring_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: spring_from_json received NULL json\n");
        return NULL;
    }
    float stiffness = 10.0f;
    cJSON *stiffness_json = cJSON_GetObjectItemCaseSensitive(json, "stiffness");
    if (stiffness_json && cJSON_IsNumber(stiffness_json)) {
        stiffness = (float)stiffness_json->valuedouble;
    }
    spring_component *comp = create_spring_component(stiffness, NULL, NULL);

    return (Component*)comp;
}

__attribute__((constructor))
static void register_spring_component() {
    component_registry_register("spring_component", spring_from_json);
}
