#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform_controller_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"
#include "input.h"

#include <cglm/cglm.h>

const float limit = 2.5f;
const float speed = 8;

void platform_controller_awake(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

void platform_controller_start(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

void platform_controller_update(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;

    network_gameObject_component *net = get_component(comp->base.gameObject, network_gameObject_component, "network_gameObject_component");
    if (net && net->owner != (network_manager_is_host() ? 0 : 1)) {
        return;
    }

    float horizontal = input_axis(SDL_SCANCODE_A, SDL_SCANCODE_D);
    float vertical = input_axis(SDL_SCANCODE_S, SDL_SCANCODE_W);

    

    transform_component *transform = get_component(comp->base.gameObject, transform_component, "transform_component");
    transform->position.z += -horizontal * speed * physics_m->timestep;
    transform->position.y += vertical * speed * physics_m->timestep;

    float bound_y = limit - transform->scale.y * 0.5f;
    float bound_z = limit - transform->scale.z * 0.5f;

    transform->position.y = min(max(transform->position.y, -bound_y), bound_y);
    transform->position.z = min(max(transform->position.z, -bound_z), bound_z);
}

void platform_controller_destroy(Component* self) {
    platform_controller_component *comp = (platform_controller_component*)self;
}

platform_controller_component *create_platform_controller_component() {
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

    return comp;
}

Component* platform_controller_from_json(cJSON *json) {
    return (Component*)create_platform_controller_component();
}

__attribute__((constructor))
static void register_platform_controller_component() {
    component_registry_register("platform_controller_component", platform_controller_from_json);
}