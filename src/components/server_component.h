#ifndef SERVER_COMPONENT_H
#define SERVER_COMPONENT_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;

} server_component;

server_component *create_server_component();

#endif
