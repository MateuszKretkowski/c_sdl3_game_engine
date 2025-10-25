#include "component_registry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

static HashMap *component_registry = NULL;

void component_registry_init() {
    if (!component_registry) {
        component_registry = hashmap_create(hash_string, compare_string);
        if (!component_registry) {
            fprintf(stderr, "component_registry_init: failed to create component registry\n");
        }
    }
}

void component_registry_register(const char *id, ComponentCreateFunc create_func) {
    if (!component_registry) {
        component_registry_init();
    }
    if (!id || !create_func) {
        fprintf(stderr, "component_registry_register: cannot register component with NULL id or function\n");
        return;
    }
    if (hashmap_contains(component_registry, (void*)id)) {
        fprintf(stderr, "component_registry_register: component '%s' already registered, overwriting\n", id);
    }
    hashmap_insert(component_registry, strdup(id), (void*)create_func);
}

Component* component_registry_create(const char *id, cJSON *json) {
    if (!component_registry) {
        component_registry_init();
    }
    if (!id) {
        fprintf(stderr, "component_registry_create: id is NULL\n");
        return NULL;
    }
    if (!hashmap_contains(component_registry, (void*)id)) {
        fprintf(stderr, "component_registry_create: unknown component type: %s\n", id);
        return NULL;
    }

    ComponentCreateFunc create_func = (ComponentCreateFunc)hashmap_get(component_registry, (void*)id);
    if (!create_func) {
        fprintf(stderr, "component_registry_create: component '%s' has NULL creation function\n", id);
        return NULL;
    }
    Component *comp = create_func(json);
    if (!comp) {
        fprintf(stderr, "component_registry_create: failed to create component: %s\n", id);
    }
    return comp;
}

void component_registry_cleanup() {
    if (component_registry) {
        // Note: We don't free the keys here as they might be string literals
        // In a production system, you'd track allocated keys separately
        hashmap_free(component_registry);
        component_registry = NULL;
    }
}
