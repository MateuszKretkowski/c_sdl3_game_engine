#ifndef RENDER_H
#define RENDER_H

#include "core/gameObject.h"

#define MAX_OBJECTS 2048

// Forward declaration - full definition in scene_manager.h
typedef struct Scene Scene;

void render_init(void);
void render_frame(void);
void render_shutdown(void);
void render_load_scene(Scene *scene);
void render_add_object(GameObject *gameObject);
void render_remove_object(GameObject *gameObject);

#endif