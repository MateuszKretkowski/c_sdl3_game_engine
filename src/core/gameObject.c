#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameObject.h"
#include "component.h"
#include <libs/cJSON.h>

GameObject *instantiate_gameObject(char* name) {
    GameObject *gameObject = malloc(sizeof(GameObject));
    gameObject->name = malloc(strlen(name) * sizeof(char) + sizeof(char));
    strcpy(gameObject->name, name);
    gameObject->components = malloc(sizeof(Component*) * 8);
    gameObject->components_capacity = 8;
    gameObject->components_length = 0;

    return gameObject;
}

void free_gameObject(GameObject *gameObject) {
    if (!gameObject) {
        return;
    }
    for (int i=0; i<gameObject->components_length; i++) {
        local_destroy_component(gameObject->components[i]);
    }
    free(gameObject->components);
    free(gameObject->id);
    free(gameObject->name);
    free(gameObject);
}

void add_component(GameObject *gameObject, Component *component) {
    if (!gameObject || !component || gameObject->components == NULL || !component) {
        return;
    }

    if (gameObject->components_length == gameObject->components_capacity) {
        gameObject->components = realloc(gameObject->components, sizeof(Component*) * gameObject->components_capacity * 2);
        gameObject->components_capacity *= 2;
    }

    gameObject->components[gameObject->components_length] = component;
    gameObject->components_length++;
}

void remove_component(GameObject *gameObject, Component *component) {
    if (!gameObject || !component || gameObject->components == NULL || !component) {
        return;
    }
    
    for (int i=0; i<gameObject->components_length; i++) {
        if (strcmp(gameObject->components[i]->name, component->name) == 0) {
            local_destroy_component(gameObject->components[i]);
            gameObject->components[i] = gameObject->components[gameObject->components_length-1];
            gameObject->components_length--;
            return;
        }
    }

    fprintf(stderr, "Could NOT find the Component to Remove: %s", component->name);
}

Component *get_component_base(GameObject *gameObject, const char *id) {
    if (!gameObject || !id) {
        return NULL;
    }
    for (int i = 0; i < gameObject->components_length; i++) {
        if (strcmp(gameObject->components[i]->id, id) == 0) {
            return gameObject->components[i];
        }
    }
    return NULL;
}

GameObject *instantiate_prefab(GameObject *prefab) {
    if (!prefab) {
        fprintf(stderr, "ERROR: instantiate_prefab received NULL prefab\n");
        return NULL;
    }

    GameObject *instance = instantiate_gameObject(prefab->name);
    if (prefab->id) {
        instance->id = strdup(prefab->id);
    }

    for (int i = 0; i < prefab->components_length; i++) {
        Component *original = prefab->components[i];
        Component *copy = malloc(original->size);
        memcpy(copy, original, original->size);
        copy->id = strdup(original->id);
        copy->name = strdup(original->name);
        copy->gameObject = instance;
        copy->standard_voids = malloc(sizeof(component_standard_voids));
        memcpy(copy->standard_voids, original->standard_voids, sizeof(component_standard_voids));
        add_component(instance, copy);
    }

    return instance;
}