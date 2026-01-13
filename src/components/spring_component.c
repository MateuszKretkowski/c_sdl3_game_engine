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
    Vector3 middle_u = vector3_divide(vector3_subtract(comp->tc_1->position, comp->tc_2->position), vector3_length(vector3_subtract(comp->tc_1->position, comp->tc_2->position)));
    float force = -(comp->stiffness * comp->length);
    add_force(comp->obj_1, middle_u, force);
    add_force(comp->obj_2, middle_u, force);
}

void spring_destroy(Component* self) {
    spring_component *comp = (spring_component*)self;
}

spring_component *create_spring_component(float stiffness, rigid_body_component *obj_1, transform_component *tc_1,  rigid_body_component *obj_2, transform_component *tc_2) {
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
    comp->tc_1 = tc_1;
    comp->obj_2 = obj_2;
    comp->tc_2 = tc_2;

    return comp;
}