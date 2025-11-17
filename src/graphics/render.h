#ifndef RENDER_H
#define RENDER_H

#include "core/gameObject.h"
#include "components/components.h"
#include "scene_manager.h"

#define MAX_OBJECTS 2048

typedef struct Scene Scene;

typedef struct {
    GameObject *gameObject;
    mesh_renderer_component *mesh_renderer_component;
    transform_component *transform_component;
} render_object;

void render_init(void);
void render_frame(void);
void render_shutdown(void);
void render_clear_stack(void);
void render_load_render_stack(Scene *scene);
void render_add_object(render_object *ro);
void render_remove_object(render_object *ro);

#endif