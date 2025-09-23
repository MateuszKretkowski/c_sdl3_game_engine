// component.h

#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdbool.h>

struct GameObject;
struct Component;

typedef struct Component {
    struct GameObject* gameObject;
    
    char *name;
    bool isActive;

    void (*start)(struct Component* self);
    void (*update)(struct Component* self);
    void (*destroy)(struct Component* self);

} Component;

void local_destroy_component(Component *component);

#endif // COMPONENT_H