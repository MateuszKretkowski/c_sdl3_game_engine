#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "utils/resource_manager.h"
#include "graphics/render.h"
#include "core/gameObject.h"

typedef struct {
    char *id;
    GameObject *gameObjects;
    int gameObjects_length;
} Scene;

#endif // SCENE_MANAGER_H