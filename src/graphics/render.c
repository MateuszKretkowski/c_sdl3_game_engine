#include <stdio.h>
#include <stdlib.h>
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

static GameObject *render_stack[MAX_OBJECTS];
static int render_stack_count;

void render_load_scene(Scene *scene) {
    if (!scene) {
        fprintf(stderr, "render_load_scene: scene is NULL\n");
        return;
    }
    if (!scene->gameObjects) {
        fprintf(stderr, "render_load_scene: scene->gameObjects is NULL\n");
        return;
    }
    if (scene->gameObjects_length > MAX_OBJECTS) {
        fprintf(stderr, "render_load_scene: scene->gameObjects_length (%d) exceeds MAX_OBJECTS (%d)\n", scene->gameObjects_length, MAX_OBJECTS);
        return;
    }
    for (int i=0; i<render_stack_count; i++) {
        free_gameObject(&scene->gameObjects[i]);
    }
    fprintf(stderr, "Loading Scene: %s\n", scene->id);
    render_stack_count = 0;
    for (int i=0; i<scene->gameObjects_length; i++) {
        fprintf(stderr, "Loading GameObject: %s\n", scene->gameObjects[i].name);
        render_stack[i] = &scene->gameObjects[i];
        render_stack_count++;
    }
}

void render_add_object(GameObject *gameObject) {
    if (render_stack_count == MAX_OBJECTS) {
        fprintf(stderr, "render_add_object: render_stack is full (MAX_OBJECTS: %d)\n", MAX_OBJECTS);
        return;
    }
    if (!gameObject) {
        fprintf(stderr, "render_add_object: gameObject is NULL\n");
        return;
    }
    render_stack[render_stack_count] = gameObject;
    render_stack_count++;
}

void render_remove_object(GameObject *gameObject) {
    if (render_stack_count == 0) {
        fprintf(stderr, "render_remove_object: render_stack is empty\n");
        return;
    }
    if (!gameObject) {
        fprintf(stderr, "render_remove_object: gameObject is NULL\n");
        return;
    }
    for (int i=0; i<render_stack_count; i++) {
        if (render_stack[i] == gameObject) {
            render_stack[i] = render_stack[render_stack_count-1];
            render_stack_count--;
            break;
        }
    }
}

void render_init() {
    render_stack_count = 0;
}

void render_frame(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i=0; i<render_stack_count; i++) {
        fprintf(stderr, "Rendering GameObject: %s\n", render_stack[i]->name);
        mesh_renderer_component *mesh_renderer = get_component(render_stack[i], mesh_renderer_component, "mesh_renderer_component");
        if (!mesh_renderer) {
            fprintf(stderr, "render_frame: GameObject '%s' has no mesh_renderer_component\n", render_stack[i]->name);
            continue;
        }
        shader_use(&mesh_renderer->mesh->materials[0]->shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh_renderer->mesh->materials[0]->diffuse_map.id);

        glBindVertexArray(mesh_renderer->mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh_renderer->mesh->indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void render_shutdown(void) {
    for (int i=0; i<render_stack_count; i++) {
        mesh_renderer_component *mesh_renderer = get_component(render_stack[i], mesh_renderer_component, "mesh_renderer_component");
        if (!mesh_renderer || !mesh_renderer->mesh->materials) {
            if (!mesh_renderer) {
                fprintf(stderr, "render_shutdown: GameObject has no mesh_renderer_component\n");
            } else if (!mesh_renderer->mesh->materials) {
                fprintf(stderr, "render_shutdown: mesh_renderer->mesh->materials is NULL\n");
            }
            continue;
        }
        glDeleteTextures(1, &mesh_renderer->mesh->materials[0]->diffuse_map.id);
        glDeleteBuffers(1, &mesh_renderer->mesh->vbo);
        glDeleteBuffers(1, &mesh_renderer->mesh->ebo);
        glDeleteVertexArrays(1, &mesh_renderer->mesh->vao);
        shader_destroy(&mesh_renderer->mesh->materials[0]->shader);
    }
}
