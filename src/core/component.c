#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "component.h"

bool Component_equals(const Component *a, const Component *b) {
    if (!a || !b) {
        fprintf(stderr, "Component_equals: invalid parameters - a: %p, b: %p\n", (void*)a, (void*)b);
        return false;
    }
    if (a->name == NULL || b->name == NULL) {
        return a->name == b->name;
    }
    return (strcmp(a->name, b->name) == 0);
}

void local_destroy_component(Component *component) {
    if (!component) {
        fprintf(stderr, "local_destroy_component: component is NULL\n");
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