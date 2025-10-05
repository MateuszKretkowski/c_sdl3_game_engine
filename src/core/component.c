#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>

struct GameObject;

typedef struct {
    struct GameObject *gameObject;

    char *name;
    bool isActive;

    component_standard_voids *standard_voids;
} Component;

typedef struct {
    void (*start)(Component *self);
    void (*update)(Component *self);
    void (*destroy)(Component *self);
} component_standard_voids;

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
    if (component->standard_voids->destroy != NULL) {
        component->standard_voids->destroy(component);
    }
    free(component->name);
    free(component);
}