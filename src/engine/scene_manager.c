#include <stdio.h>
#include <stdlib.h>
#include "scene_manager.h"

Scene *current_scene;
camera_component *active_camera;

void scene_manager_init() {
    current_scene = NULL;
    active_camera = NULL;
}

camera_component *render_get_active_camera() {
    for (int i=0; i<current_scene->gameObjects_length; i++) {
        if (current_scene->gameObjects[i].id && current_scene->gameObjects[i].id && strcmp(current_scene->gameObjects[i].id, "camera") == 0) {
            camera_component *cc = get_component(&current_scene->gameObjects[i], camera_component, "camera_component");
            if (cc) {
                active_camera = cc;
                return cc;
            }
        }
    }
    fprintf(stderr, "ERROR: No active camera found in scene!\n");
    return NULL;
}

void load_scene(char *id) {
    if (!current_scene) {
        scene_manager_init();
    }
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
        // Clear render stack before freeing old scene
        render_clear_stack();
        for (int i=0; i<current_scene->gameObjects_length; i++) {
            free_gameObject(&current_scene->gameObjects[i]);
        }
    }
    current_scene = scene;

    // Resolve camera target references before awake/start
    fprintf(stderr, "load_scene: Resolving camera target references\n");
    for (int i=0; i<scene->gameObjects_length; i++) {
        camera_component *cam = get_component(&scene->gameObjects[i], camera_component, "camera_component");
        if (cam && cam->target_id) {
            fprintf(stderr, "load_scene: Found camera with target_id '%s'\n", cam->target_id);
            // Find the target GameObject by ID
            for (int j=0; j<scene->gameObjects_length; j++) {
                if (scene->gameObjects[j].id && strcmp(scene->gameObjects[j].id, cam->target_id) == 0) {
                    transform_component *target_transform = get_component(&scene->gameObjects[j], transform_component, "transform_component");
                    if (target_transform) {
                        cam->target = target_transform;
                        fprintf(stderr, "load_scene: Resolved camera target to GameObject '%s'\n", scene->gameObjects[j].id);
                    } else {
                        fprintf(stderr, "ERROR: Target GameObject '%s' has no transform_component\n", cam->target_id);
                    }
                    break;
                }
            }
            if (!cam->target) {
                fprintf(stderr, "ERROR: Camera target '%s' not found in scene\n", cam->target_id);
            }
        }
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
    
    for (int i=0; i<current_scene->gameObjects_length; i++) {
        render_object *ro = malloc(sizeof(render_object));
        ro->gameObject = &current_scene->gameObjects[i];
        ro->mesh_renderer_component = get_component(&current_scene->gameObjects[i], mesh_renderer_component, "mesh_renderer_component");
        ro->transform_component = get_component(&current_scene->gameObjects[i], transform_component, "transform_component");
        render_add_object(ro);
    }
}     