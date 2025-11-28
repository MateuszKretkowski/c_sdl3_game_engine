#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "utils/resource_manager.h"
#include "graphics/render.h"
#include "core/gameObject.h"
#include "components/components.h"

typedef struct Scene {
    char *id;
    GameObject *gameObjects;
    int gameObjects_length;
    cJSON *scene_config;
} Scene;

cJSON *scene_get_scene_config();
Scene *scene_get_scene();

void load_scene(char *id);
camera_component *render_get_active_camera();

#endif // SCENE_MANAGER_H