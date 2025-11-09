#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameObject.h"
#include "component.h"
#include <libs/cJSON.h>

GameObject *instantiate_gameObject(char* id) {
    GameObject *gameObject = malloc(sizeof(GameObject));
    if (!gameObject) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for GameObject\n");
        return NULL;
    }
    gameObject->id = malloc(strlen(id) * sizeof(char) + sizeof(char));
    if (!gameObject->id) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for id: %s\n", id);
        free(gameObject);
        return NULL;
    }
    gameObject->name = NULL;
    

    strcpy(gameObject->id, id);
    gameObject->components = malloc(sizeof(Component*) * 8);
    if (!gameObject->components) {
        fprintf(stderr, "instantiate_gameObject: failed to allocate memory for components array\n");
        free(gameObject->id);
        free(gameObject);
        return NULL;
    }
    gameObject->components_capacity = 8;
    gameObject->components_length = 0;

    return gameObject;
}

void free_gameObject(GameObject *gameObject) {
    if (!gameObject) {
        fprintf(stderr, "free_gameObject: gameObject is NULL\n");
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
        fprintf(stderr, "add_component: invalid parameters - gameObject: %p, component: %p\n", (void*)gameObject, (void*)component);
        return;
    }

    if (gameObject->components_length == gameObject->components_capacity) {
        Component **new_components = realloc(gameObject->components, sizeof(Component*) * gameObject->components_capacity * 2);
        if (!new_components) {
            fprintf(stderr, "add_component: failed to reallocate components array\n");
            return;
        }
        gameObject->components = new_components;
        gameObject->components_capacity *= 2;
    }

    component->gameObject = gameObject;
    gameObject->components[gameObject->components_length] = component;
    gameObject->components_length++;
}

void remove_component(GameObject *gameObject, Component *component) {
    if (!gameObject || !component || gameObject->components == NULL || !component) {
        fprintf(stderr, "remove_component: invalid parameters - gameObject: %p, component: %p\n", (void*)gameObject, (void*)component);
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

    fprintf(stderr, "remove_component: could not find component to remove: %s\n", component->name);
}

Component *get_component_base(GameObject *gameObject, const char *id) {
    if (!gameObject || !id) {
        fprintf(stderr, "get_component_base: invalid parameters - gameObject: %p, id: %p\n", (void*)gameObject, (void*)id);
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
        fprintf(stderr, "instantiate_prefab: prefab is NULL\n");
        return NULL;
    }

    GameObject *instance = instantiate_gameObject(prefab->name);
    if (!instance) {
        fprintf(stderr, "instantiate_prefab: failed to instantiate gameObject\n");
        return NULL;
    }
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