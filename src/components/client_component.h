#ifndef CLIENT_COMPONENT_H
#define CLIENT_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} client_component;

client_component *create_client_component();

#endif
