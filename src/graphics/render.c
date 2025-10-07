#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "shader.h"
#include "mesh.h"
#include "graphics.h"
#include "textures.h"
#include "utils.h"

static Shader standard_shader;
static GLuint vao, vbo, ebo;
static GLuint bricks_texture, wood_texture;

void render_init(void) {
    // Załaduj shader
    standard_shader = shader_create("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", "standard_shader");
    if (!standard_shader.id) {
        fprintf(stderr, "[render] Failed to load shader\n");
        exit(EXIT_FAILURE);
    }

    // Załaduj tekstury
    bricks_texture = create_texture("brick_texture/bricks_color.png");
    if (!bricks_texture) {
        fprintf(stderr, "[render] Failed to load brick texture\n");
        exit(EXIT_FAILURE);
    }

    wood_texture = create_texture("wood_texture/wood_color.png");
    if (!wood_texture) {
        fprintf(stderr, "[render] Failed to load wood texture\n");
        exit(EXIT_FAILURE);
    }

    // Dane do siatki
    Vertex vertices[] = {
        { { 0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { {-0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {-0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Tworzenie buforów
    vbo = create_vbo(vertices, sizeof(vertices) / sizeof(Vertex));
    if (!vbo) {
        fprintf(stderr, "[render] Failed to create VBO\n");
        exit(EXIT_FAILURE);
    }

    ebo = create_ebo(indices, sizeof(indices) / sizeof(GLuint));
    if (!ebo) {
        fprintf(stderr, "[render] Failed to create EBO\n");
        exit(EXIT_FAILURE);
    }

    vao = create_vao(vbo, &ebo);
    if (!vao) {
        fprintf(stderr, "[render] Failed to create VAO\n");
        exit(EXIT_FAILURE);
    }
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
