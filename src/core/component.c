#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct GameObject;

typedef struct Component Component;

typedef struct {
    void (*awake)(Component *self);
    void (*start)(Component *self);
    void (*update)(Component *self);
    void (*destroy)(Component *self);
} component_standard_voids;

typedef struct Component {
    struct GameObject *gameObject;
    
    char *id;
    char *name;
    bool isActive;
    
    component_standard_voids *standard_voids;
} Component;

bool Component_equals(const Component *a, const Component *b) {
    if (a->name == NULL || b->name == NULL) {
        return a->name == b->name;
    }
    return (strcmp(a->name, b->name) == 0);
}

void local_destroy_component(Component *component) {
    if (!component) {
        return;
    }
    if (component->standard_voids && component->standard_voids->destroy != NULL) {
        component->standard_voids->destroy(component);
    }
    free(component->id);
    free(component->name);
    free(component->standard_voids);
    free(component);
}