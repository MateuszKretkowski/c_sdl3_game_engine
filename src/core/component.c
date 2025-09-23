#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>

struct GameObject;

typedef struct {
    struct GameObject *gameObject;

    char *name;
    bool isActive;

    void (*start)(Component *self);
    void (*update)(Component *self);
    void (*destroy)(Component *self);
} Component;

void local_destroy_component(Component *component) {
    if (!component) {
        return;
    }
    if (component->destroy != NULL) {
        component->destroy(component);
    }
    free(component->name);
    free(component);
}