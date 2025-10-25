#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "utils/resource_manager.h"
#include "graphics/render.h"
#include "core/gameObject.h"

typedef struct Scene {
    char *id;
    GameObject *gameObjects;
    int gameObjects_length;
} Scene;

void load_scene(char *id);

#endif // SCENE_MANAGER_H