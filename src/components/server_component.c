#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "server_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "engine/input.h"

#include <cglm/cglm.h>

void server_awake(Component* self) {
    server_component *comp = (server_component*)self;
}

void server_start(Component* self) {
    server_component *comp = (server_component*)self;
}

void server_update(Component* self) {
    server_component *comp = (server_component*)self;
}

void server_destroy(Component* self) {
    server_component *comp = (server_component*)self;
}

server_component *create_server_component() {
    server_component* comp = malloc(sizeof(server_component));
    comp->base.id = strdup("server_component");
    comp->base.name = strdup("server");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(server_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = server_awake;
    comp->base.standard_voids->start = server_start;
    comp->base.standard_voids->update = server_update;
    comp->base.standard_voids->destroy = server_destroy;

    return comp;
}

Component* server_from_json(cJSON *json) {
    return (Component*)create_server_component();
}

__attribute__((constructor))
static void register_server_component() {
    component_registry_register("server_component", server_from_json);
}
