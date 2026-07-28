#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform_controller_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void platform_controller_awake(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

void platform_controller_start(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

void platform_controller_update(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

void platform_controller_destroy(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

platform_controller_component *create_platform_controller_component(float stiffness, rigid_body_component *obj_1, transform_component *tc_1,  rigid_body_component *obj_2, transform_component *tc_2) {
    platform_controller_component* comp = malloc(sizeof(platform_controller_component));
    comp->base.id = strdup("platform_controller_component");
    comp->base.name = strdup("platform_controller");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(platform_controller_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = platform_controller_awake;
    comp->base.standard_voids->start = platform_controller_start;
    comp->base.standard_voids->update = platform_controller_update;
    comp->base.standard_voids->destroy = platform_controller_destroy;

    comp->stiffness = stiffness;

    comp->obj_1 = obj_1;
    comp->tc_1 = tc_1;
    comp->obj_2 = obj_2;
    comp->tc_2 = tc_2;

    return comp;
}
