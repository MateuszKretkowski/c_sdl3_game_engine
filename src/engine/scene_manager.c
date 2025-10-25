#include <stdio.h>
#include <stdlib.h>
#include "scene_manager.h"

void load_scene(char *id) {
    if (!id) {
        fprintf(stderr, "load_scene: id is NULL\n");
        return;
    }
    fprintf(stderr, "load_scene: Loading scene with id: %s\n", id);
    Scene *scene = resource_get_scene(id);
    if (!scene) {
        fprintf(stderr, "load_scene: resource_get_scene failed for id: %s\n", id);
        return;
    }
    fprintf(stderr, "load_scene: Scene loaded successfully, calling render_load_scene\n");
    render_load_scene(scene);
    fprintf(stderr, "load_scene: render_load_scene completed\n");
}