#include <stdio.h>
#include <stdlib.h>
#include "scene_manager.h"

void load_scene(char *id) {
    if (!id) {
        fprintf(stderr, "ERROR: no id when supposed to load scene id: %s", id);
        return;
    }
    Scene *scene = resource_get_scene(id);
    render_load_scene(scene);
}