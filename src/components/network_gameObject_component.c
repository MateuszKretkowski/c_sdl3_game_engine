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
}

void network_gameObject_update(Component* self) {
    network_gameObject_component *comp = (network_gameObject_component*)self;

    transform_component *transform = get_component(comp->base.gameObject, transform_component, "transform_component");

    unsigned char *positionTB = malloc(sizeof(transform->position));
    positionTB[0] = (unsigned char) &transform->position.x;
    positionTB[sizeof(int)] = (unsigned char) &transform->position.y;
    positionTB[sizeof(int)*2] = (unsigned char) &transform->position.z;

    unsigned char *rotationTB = malloc(sizeof(transform->rotation));
    rotationTB[0] = (unsigned char) &transform->rotation.x;
    rotationTB[sizeof(float)] = (unsigned char) &transform->rotation.y;
    rotationTB[sizeof(float)*2] = (unsigned char) &transform->rotation.z;

    unsigned char *scaleTB = malloc(sizeof(transform->scale));
    scaleTB[0] = (unsigned char) &transform->scale.x;
    scaleTB[sizeof(float)] = (unsigned char) &transform->scale.y;
    scaleTB[sizeof(float)*2] = (unsigned char) &transform->scale.z;
}

void network_gameObject_destroy(Component* self) {
    network_gameObject_component *comp = (network_gameObject_component*)self;
}

network_gameObject_component *create_network_gameObject_component() {
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

    return comp;
}

Component* network_gameObject_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: network_gameObject_from_json received NULL json\n");
        return NULL;
    }

    return (Component*)create_network_gameObject_component();
}

__attribute__((constructor))
static void register_network_gameObject_component() {
    component_registry_register("network_gameObject_component", network_gameObject_from_json);
}
