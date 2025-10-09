#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include <libs/cJSON.h>
#include "core/component.h"

// Function pointer type for component creation from JSON
typedef Component* (*ComponentCreateFunc)(cJSON* json);

// Initialize the component registry
void component_registry_init();

// Register a component creation function
void component_registry_register(const char *id, ComponentCreateFunc create_func);

// Create a component by ID from JSON data
Component* component_registry_create(const char *id, cJSON *json);

// Cleanup the registry
void component_registry_cleanup();

#endif // COMPONENT_REGISTRY_H
