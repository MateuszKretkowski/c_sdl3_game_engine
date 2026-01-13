#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdbool.h>
#include "component.h"

typedef struct GameObject {
    char *id;
    char *name;

    GameObject **gameObjects;
    Component **components;

    int gameObjects_capacity;
    int gameObjects_length;

    int components_capacity;
    int components_length;

} GameObject;

GameObject *instantiate_gameObject(char* id);
GameObject *instantiate_prefab(GameObject *prefab);
void free_gameObject(GameObject *gameObject);
void add_component(GameObject *gameObject, Component *component);
void remove_component_by_id(GameObject *gameObject, const char *id);
void remove_component(GameObject *gameObject, Component *component);
void add_child(GameObject *parent, GameObject *child);
void remove_child(GameObject *parent, int index);
void remove_child_by_gameObject(GameObject *parent, GameObject *child);
GameObject *get_child(GameObject *gameObject, int index);
GameObject **get_children(GameObject *gameObject);
Component *get_component_base(GameObject *gameObject, const char *id);

#define get_component(gameObject, ComponentType, id) \
    ((ComponentType*)get_component_base(gameObject, id))

#endif // GAMEOBJECT_H