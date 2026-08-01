#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "network_manager_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "engine/input.h"

#include <cglm/cglm.h>

bool isServer;
bool isGameActive;
bool isReady;

int port;
char ip[64];

void network_manager_awake(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    isServer = false;
    isGameActive = false;
    isReady = false;
}

void network_manager_start(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    char buf[64];

    printf("isServer (1 or 0): ");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin)) {
        isServer = atoi(buf);
    }

    printf("Set Port: ");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin)) {
        port = atoi(buf);
    }

    if (!isServer) {
        printf("Set IP: ");
        fflush(stdout);
        if (fgets(ip, sizeof(ip), stdin)) {
            ip[strcspn(ip, "\r\n")] = '\0';
        }
    }
    
    isReady = true;
}

void network_manager_update(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    if (isReady) {
        load_scene("pong_scene");
    }
}

void network_manager_destroy(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;
}

network_manager_component *create_network_manager_component() {
    network_manager_component* comp = malloc(sizeof(network_manager_component));
    comp->base.id = strdup("network_manager_component");
    comp->base.name = strdup("network_manager");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(network_manager_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = network_manager_awake;
    comp->base.standard_voids->start = network_manager_start;
    comp->base.standard_voids->update = network_manager_update;
    comp->base.standard_voids->destroy = network_manager_destroy;

    return comp;
}

Component* network_manager_from_json(cJSON *json) {
    return (Component*)create_network_manager_component();
}

__attribute__((constructor))
static void register_network_manager_component() {
    component_registry_register("network_manager_component", network_manager_from_json);
}
