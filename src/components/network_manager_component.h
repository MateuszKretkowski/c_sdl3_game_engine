#ifndef NETWORK_MANAGER_COMPONENT_H
#define NETWORK_MANAGER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} network_manager_component;

void network_manager_register(GameObject *gameObject);
void network_manager_unregister(GameObject *gameObject);
bool network_manager_is_host();

network_manager_component *create_network_manager_component();

#endif
