#include <stdio.h>
#include <stdlib.h>
#include "scene_manager.h"

const Scene *current_scene;

void scene_manager_init() {
    current_scene = NULL;
}

void load_scene(char *id) {
    if (!id) {
        fprintf(stderr, "load_scene: id is NULL\n");
        return;
    }

    Scene *scene = resource_get_scene(id);
    if (!scene) {
        fprintf(stderr, "ERROR: Failed to load scene '%s'\n", id);
        return;
    }

    if (current_scene) {
        for (int i=0; i<current_scene->gameObjects_length; i++) {
            free_gameObject(&scene->gameObjects[i]);
        }
    }
    current_scene = scene;
    for (int i=0; i<current_scene->gameObjects_length; i++) {
        render_object *ro;
        ro->gameObject = &current_scene->gameObjects[i];
        ro->mesh_renderer_component = get_component(&current_scene->gameObjects[i], mesh_renderer_component, "mesh_renderer_component");
        ro->transform_component = get_component(&current_scene->gameObjects[i], transform_component, "transform_component");
        
    }

    for (int i=0; i<scene->gameObjects_length; i++) {
        for (int j=0; j<scene->gameObjects[i].components_length; j++) {
            Component *comp = scene->gameObjects[i].components[j];
            if (comp && comp->standard_voids && comp->standard_voids->awake) {
                comp->standard_voids->awake((Component*)comp);
            }
        }
    }

    for (int i=0; i<scene->gameObjects_length; i++) {
        for (int j=0; j<scene->gameObjects[i].components_length; j++) {
            Component *comp = scene->gameObjects[i].components[j];
            if (comp && comp->standard_voids && comp->standard_voids->start) {
                comp->standard_voids->start((Component*)comp);
            }
        }
    }

    render_load_scene(scene);
}