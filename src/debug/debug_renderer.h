#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include <stdbool.h>
#include "utils.h"
#include "components/components.h"
#include "utils/resource_manager.h"

typedef struct render_object render_object;

typedef struct {
    render_object *ro;
    float time_left;
} debug_object;

render_object *debug_get_objects();
int debug_get_count();
void debug_draw_sphere(Vector3 pos, float radius, Vector4 color, float time);

#endif