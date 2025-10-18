#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdbool.h>
#include "component.h"

typedef struct GameObject {
    char *id;
    char *name;

    Component **components;

    int components_capacity;
    int components_length;

} GameObject;

GameObject *instantiate_gameObject(char* name);
GameObject *instantiate_prefab(GameObject *prefab);
void free_gameObject(GameObject *gameObject);
void add_component(GameObject *gameObject, Component *component);
void remove_component(GameObject *gameObject, Component *component);
Component *get_component_base(GameObject *gameObject, const char *id);

#define get_component(gameObject, ComponentType, id) \
    ((ComponentType*)get_component_base(gameObject, id))

#endif // GAMEOBJECT_H