#include <stdio.h>
#include <stdlib.h>
#include "scene_manager.h"

void load_scene(char *id) {
    if (!id) {
        fprintf(stderr, "load_scene: id is NULL\n");
        return;
    }
    Scene *scene = resource_get_scene(id);
    if (!scene) {
        fprintf(stderr, "load_scene: resource_get_scene failed for id: %s\n", id);
        return;
    }
    render_load_scene(scene);
}