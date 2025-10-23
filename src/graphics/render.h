#ifndef RENDER_H
#define RENDER_H

#include "resource_manager.h"
#include "core/gameObject.h"

#define MAX_OBJECTS 2048

void render_init(void);
void render_frame(void);
void render_shutdown(void);
void render_load_scene(Scene *scene);
void render_add_object(GameObject *gameObject);
void render_remove_object(GameObject *gameObject);

#endif