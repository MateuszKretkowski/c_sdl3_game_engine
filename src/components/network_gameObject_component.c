#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network_gameObject_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"

network_manager_component *network_component;

void network_gameObject_awake(Component* self) {
    network_gameObject_component *comp = (network_gameObject_component*)self;
}

void network_gameObject_start(Component* self) {
    network_gameObject_component *comp = (network_gameObject_component*)self;

    GameObject *network_manager = scene_get_gameObject("network_manager");
    if (!network_manager) {
        printf("ERROR: network_manager does not exist.\n");
        return;
    }
    network_component = get_component(network_manager, network_manager_component, "network_manager_component");
    if (!network_component) {
        printf("ERROR: network_manager has no network_manager_component.\n");
        return;
    }

    network_manager_register(comp->base.gameObject);
}

void network_gameObject_update(Component* self) {
    network_gameObject_component *comp = (network_gameObject_component*)self;

    transform_component *transform = get_component(comp->base.gameObject, transform_component, "transform_component");

    memcpy(comp->buffer, &transform->position, sizeof(Vector3));
    memcpy(comp->buffer + sizeof(Vector3), &transform->rotation, sizeof(Vector3));
    memcpy(comp->buffer + sizeof(Vector3) * 2, &transform->scale, sizeof(Vector3));
}

void network_gameObject_destroy(Component* self) {
    network_gameObject_component *comp = (network_gameObject_component*)self;
    network_manager_unregister(comp->base.gameObject);
}

network_gameObject_component *create_network_gameObject_component(int owner) {
    network_gameObject_component* comp = malloc(sizeof(network_gameObject_component));
    comp->base.id = strdup("network_gameObject_component");
    comp->base.name = strdup("Network GameObject");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(network_gameObject_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = network_gameObject_awake;
    comp->base.standard_voids->start = network_gameObject_start;
    comp->base.standard_voids->update = network_gameObject_update;
    comp->base.standard_voids->destroy = network_gameObject_destroy;

    comp->owner = owner;

    return comp;
}

Component* network_gameObject_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: network_gameObject_from_json received NULL json\n");
        return NULL;
    }

    int owner = 0;

    cJSON *owner_json = cJSON_GetObjectItemCaseSensitive(json, "owner");
    if (owner_json && cJSON_IsNumber(owner_json)) {
        owner = owner_json->valueint;
    }

    return (Component*)create_network_gameObject_component(owner);
}

__attribute__((constructor))
static void register_network_gameObject_component() {
    component_registry_register("network_gameObject_component", network_gameObject_from_json);
}
