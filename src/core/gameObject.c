#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component.h"

typedef struct {
    char *name;

    Component **components;

    int components_capacity;
    int components_length;
    
} GameObject;

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
    for (int i=0; i<gameObject->components_length; i++) {
        global_destroy_component(gameObject->components[i]);
    }
    free(gameObject->components);
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
            global_destroy_component(gameObject->components[i]);
            gameObject->components[i] = gameObject->components[gameObject->components_length-1];
            gameObject->components_length--;
            return;
        }
    }

    fprintf(stderr, "Could NOT find the Component to Remove: %s", component->name);
}