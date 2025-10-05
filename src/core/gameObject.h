#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdbool.h>
#include "component.h"

typedef struct {
    char *name;

    Component **components;

    int components_capacity;
    int components_length;
    
} GameObject;

GameObject *instantiate_gameObject(char* name);
void free_gameObject(GameObject *gameObject);
void add_component(GameObject *gameObject, Component *component);
void remove_component(GameObject *gameObject, Component *component);
Component *get_component(GameObject *gameObject, Component component);

#endif // GAMEOBJECT_H