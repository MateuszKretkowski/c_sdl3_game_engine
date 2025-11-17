#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "shader.h"
#include "mesh.h"
#include "graphics.h"
#include "textures.h"
#include "utils.h"
#include "core/gameObject.h"
#include "core/component.h"
#include "resource_manager.h"
#include "components/components.h"
#include "engine/scene_manager.h"

static render_object *render_stack[MAX_OBJECTS];
static int render_stack_count;

void render_add_object(render_object *render_object) {
    if (render_stack_count == MAX_OBJECTS) {
        fprintf(stderr, "render_add_object: render_stack is full (MAX_OBJECTS: %d)\n", MAX_OBJECTS);
        return;
    }
    if (!render_object) {
        fprintf(stderr, "render_add_object: gameObject is NULL\n");
        return;
    }
    render_stack[render_stack_count] = render_object;
    render_stack_count++;

    
}

void render_remove_object(render_object *render_object) {
    if (render_stack_count == 0) {
        fprintf(stderr, "render_remove_object: render_stack is empty\n");
        return;
    }
    if (!render_object) {
        fprintf(stderr, "render_remove_object: gameObject is NULL\n");
        return;
    }
    for (int i=0; i<render_stack_count; i++) {
        if (render_stack[i] == render_object) {
            render_stack[i] = render_stack[render_stack_count-1];
            render_stack_count--;
            break;
        }
    }
}

void render_init(void) {
    render_stack_count = 0;
    glEnable(GL_DEPTH_TEST);
    fprintf(stderr, "=== Render System Initialized ===\n");
}

void render_clear_stack(void) {
    fprintf(stderr, "render_clear_stack: Clearing %d render objects\n", render_stack_count);
    for (int i = 0; i < render_stack_count; i++) {
        if (render_stack[i]) {
            free(render_stack[i]);
            render_stack[i] = NULL;
        }
    }
    render_stack_count = 0;
}

void render_frame(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (render_stack_count < 1) {
        return;
    }
    
    for (int i=0; i<render_stack_count; i++) {
        mesh_renderer_component *mesh_renderer = render_stack[i]->mesh_renderer_component;
        if (!mesh_renderer) {
            continue;
        }
        if (!mesh_renderer->mesh) {
            fprintf(stderr, "render_frame: GameObject '%s' has mesh_renderer but mesh is NULL\n", render_stack[i]->gameObject->name);
            continue;
        }
        if (!mesh_renderer->mesh->materials) {
            fprintf(stderr, "render_frame: GameObject '%s' mesh has no materials array\n", render_stack[i]->gameObject->name);
            continue;
        }
        if (mesh_renderer->mesh->materials[0] == NULL) {
            fprintf(stderr, "render_frame: GameObject '%s' mesh materials[0] is NULL\n", render_stack[i]->gameObject->name);
            continue;
        }
        transform_component *transform = render_stack[i]->transform_component;
        if (!transform) {
            fprintf(stderr, "render_frame: GameObject '%s' has no transform_component\n", render_stack[i]->gameObject->name);
            continue;
        }
        shader_use(&mesh_renderer->mesh->materials[0]->shader);

        camera_component *active_camera = render_get_active_camera();
        if (!active_camera) {
            fprintf(stderr, "render_frame: No active camera found, skipping rendering for '%s'\n", render_stack[i]->gameObject->name);
            continue;
        }

        GLuint transformLoc = glGetUniformLocation(mesh_renderer->mesh->materials[0]->shader.id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)transform->model);

        GLuint viewLoc = glGetUniformLocation(mesh_renderer->mesh->materials[0]->shader.id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)active_camera->view);

        GLuint projectionLoc = glGetUniformLocation(mesh_renderer->mesh->materials[0]->shader.id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)active_camera->projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh_renderer->mesh->materials[0]->diffuse_map.id);

        glBindVertexArray(mesh_renderer->mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh_renderer->mesh->indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    for (int i=0; i<render_stack_count; i++) {
        GameObject *gameObject = render_stack[i]->gameObject;
        for (int j=0; j<gameObject->components_length; j++) {
            Component *comp = gameObject->components[j];
            if (comp && comp->standard_voids && comp->standard_voids->update) {
                comp->standard_voids->update((Component*)comp);
            }
        }
    }
}

void render_shutdown(void) {
    for (int i=0; i<render_stack_count; i++) {
        GameObject *gameObject = render_stack[i]->gameObject;
        for (int j=0; j<gameObject->components_length; j++) {
            Component *comp = gameObject->components[j];
            if (comp && comp->standard_voids && comp->standard_voids->destroy) {
                comp->standard_voids->destroy((Component*)comp);
            }
        }
    }
    
    for (int i=0; i<render_stack_count; i++) {
        mesh_renderer_component *mesh_renderer = render_stack[i]->mesh_renderer_component;
        if (!mesh_renderer) {
            fprintf(stderr, "render_shutdown: GameObject has no mesh_renderer_component\n");
            continue;
        }
        if (!mesh_renderer->mesh) {
            fprintf(stderr, "render_shutdown: mesh_renderer->mesh is NULL\n");
            continue;
        }
        if (!mesh_renderer->mesh->materials) {
            fprintf(stderr, "render_shutdown: mesh_renderer->mesh->materials is NULL\n");
            continue;
        }
        glDeleteTextures(1, &mesh_renderer->mesh->materials[0]->diffuse_map.id);
        glDeleteBuffers(1, &mesh_renderer->mesh->vbo);
        glDeleteBuffers(1, &mesh_renderer->mesh->ebo);
        glDeleteVertexArrays(1, &mesh_renderer->mesh->vao);
        shader_destroy(&mesh_renderer->mesh->materials[0]->shader);
    }
}
