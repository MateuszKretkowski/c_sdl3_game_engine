#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdbool.h>
#include "component.h"

typedef struct {
    Component *components;
} GameObject;

#endif // GAMEOBJECT_H