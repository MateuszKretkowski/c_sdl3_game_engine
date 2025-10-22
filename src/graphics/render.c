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

static GameObject *render_stack[MAX_OBJECTS];
static int render_stack_count;

void render_load_scene(Scene *scene) {
    if (render_stack_count == MAX_OBJECTS) {
        return;
    }
    if (!scene->gameObjects || scene || scene->id) {
        return;
    }
    for (int i=0; i<render_stack_count; i++) {
        free_gameObject(&scene->gameObjects[i]);
    }
    for (int i=0; i<scene->gameObjects_length; i++) {
        render_stack[i] = &scene->gameObjects[i];
    }
}

void render_add_object(GameObject *gameObject) {
    if (render_stack_count == MAX_OBJECTS) {
        return;
    }
    if (!gameObject) {
        return;
    }
    render_stack_count++;
    render_stack[render_stack_count] = gameObject;
}

void render_remove_object(GameObject *gameObject) {
    if (render_stack_count == 0) {
        return;
    }
    if (!gameObject) {
        return;
    }
    for (int i=0; i<render_stack_count; i++) {
        if (render_stack[i] == gameObject) {
            render_stack[i] = render_stack[render_stack_count-1];
            render_stack_count--;
        }
    }
}

void render_init() {
    
}

void render_frame(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_use(&standard_shader);
    shader_set_float(&standard_shader, "horizontal_offset", 0.5f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bricks_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, wood_texture);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void render_shutdown(void) {
    glDeleteTextures(1, &bricks_texture);
    glDeleteTextures(1, &wood_texture);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    shader_destroy(&standard_shader);
}
