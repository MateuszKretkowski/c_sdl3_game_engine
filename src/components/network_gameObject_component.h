#ifndef NETWORK_GAMEOBJECT_COMPONENT_H
#define NETWORK_GAMEOBJECT_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

    int owner;
    unsigned char buffer[32 + sizeof(Vector3) * 3];
} network_gameObject_component;

network_gameObject_component *create_network_gameObject_component(int owner);

#endif
