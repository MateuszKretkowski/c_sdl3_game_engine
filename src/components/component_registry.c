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
            fprintf(stderr, "ERROR: Failed to create component registry\n");
        }
    }
}

void component_registry_register(const char *id, ComponentCreateFunc create_func) {
    if (!component_registry) {
        component_registry_init();
    }
    if (!id || !create_func) {
        fprintf(stderr, "ERROR: Cannot register component with NULL id or function\n");
        return;
    }
    if (hashmap_contains(component_registry, (void*)id)) {
        fprintf(stderr, "WARNING: Component '%s' already registered, overwriting\n", id);
    }
    hashmap_insert(component_registry, strdup(id), (void*)create_func);
    printf("Registered component: %s\n", id);
}

Component* component_registry_create(const char *id, cJSON *json) {
    if (!component_registry) {
        component_registry_init();
    }
    if (!id) {
        fprintf(stderr, "ERROR: Cannot create component with NULL id\n");
        return NULL;
    }
    if (!hashmap_contains(component_registry, (void*)id)) {
        fprintf(stderr, "ERROR: Unknown component type: %s\n", id);
        return NULL;
    }

    ComponentCreateFunc create_func = (ComponentCreateFunc)hashmap_get(component_registry, (void*)id);
    if (!create_func) {
        fprintf(stderr, "ERROR: Component '%s' has NULL creation function\n", id);
        return NULL;
    }
    return create_func(json);
}

void component_registry_cleanup() {
    if (component_registry) {
        // Note: We don't free the keys here as they might be string literals
        // In a production system, you'd track allocated keys separately
        hashmap_free(component_registry);
        component_registry = NULL;
    }
}
