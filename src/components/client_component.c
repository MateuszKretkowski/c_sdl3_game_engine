#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "client_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "engine/input.h"

#include <cglm/cglm.h>

void client_awake(Component* self) {
    client_component *comp = (client_component*)self;
}

void client_start(Component* self) {
    client_component *comp = (client_component*)self;
}

void client_update(Component* self) {
    client_component *comp = (client_component*)self;
}

void client_destroy(Component* self) {
    client_component *comp = (client_component*)self;
}

client_component *create_client_component() {
    client_component* comp = malloc(sizeof(client_component));
    comp->base.id = strdup("client_component");
    comp->base.name = strdup("client");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(client_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = client_awake;
    comp->base.standard_voids->start = client_start;
    comp->base.standard_voids->update = client_update;
    comp->base.standard_voids->destroy = client_destroy;

    return comp;
}

Component* client_from_json(cJSON *json) {
    return (Component*)create_client_component();
}

__attribute__((constructor))
static void register_client_component() {
    component_registry_register("client_component", client_from_json);
}
