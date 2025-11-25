#include "physics_manager.h"

GameObject *physics_gameObjects;
int physics_gameObjects_length;
int physics_gameObjects_capacity;

void physics_manager_init() {
    physics_gameObjects = malloc(sizeof(GameObject*) * 1024);
    physics_gameObjects_length = 0;
    physics_gameObjects_capacity = 1024;
}

void physics_manager_add(GameObject *gameObject) {
    if (!gameObject || !gameObject->id || !get_component(gameObject, rigid_body_component, "rigid_body_component")) {
        return;
    }
    if (physics_gameObjects_length == physics_gameObjects_capacity) {
        physics_gameObjects_capacity *= 2;
        realloc(physics_gameObjects, sizeof(GameObject*)*physics_gameObjects_capacity);
    }
    physics_gameObjects[physics_gameObjects_length] = *gameObject;
    physics_gameObjects_length++;
}

void physics_manager_remove(GameObject *gameObject) {
    if (!gameObject || !gameObject->id || !get_component(gameObject, rigid_body_component, "rigid_body_component")) {
        return;
    }
    if (physics_gameObjects_length == 0) {
        return;
    }
    for (int i=0; i<physics_gameObjects_length; i++) {
        if (&physics_gameObjects[i] == gameObject) {
            physics_gameObjects[i] = physics_gameObjects[physics_gameObjects_length];
            physics_gameObjects_length--;
        }
    }
}

void physics_manager_update() {
    // spatial system, itd..
}