// component.h

#ifndef COMPONENT_H
#define COMPONENT_H

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
    struct GameObject* gameObject;

    char *id;
    char *name;
    bool isActive;
    size_t size;

    component_standard_voids *standard_voids;
} Component;

bool Component_equals(const Component *a, const Component *b);
void local_destroy_component(Component *component);

#endif // COMPONENT_H