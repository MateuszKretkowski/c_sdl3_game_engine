#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include <windows.h>
#include "graphics.h"
#include "shader.h"
#include "utils.h"
#include "mesh.h"
#include "math.h"
#include "textures.h"

int main(int argc, char *argv[]) {
    SetCurrentDirectoryA("..");
    
    if (SDL_Init(SDL_INIT_VIDEO) != 1) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow(
        "atiEngine",
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        fprintf(stderr, "Error SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        fprintf(stderr, "Error SDL_GLContext: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        return 1;
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    SDL_GL_SetSwapInterval(1); // VSync = ON

    // TEXTURES
    GLuint bricks_texture = generate_texture("brick_texture/bricks_color.png");
    GLuint wood_texture = generate_texture("wood_texture/wood_color.png");
    
    int indexCount = 6;

    Vertex vertices[] = {
        { { 0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }, // top right
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // bottom right
        { {-0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // bottom left
        { {-0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }  // top left
    };

    GLuint indices[] = {
        0, 1, 3,  // pierwszy trójkąt: top right, bottom right, top left
        1, 2, 3   // drugi trójkąt: bottom right, bottom left, top left
    };

    float texCoords[] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };
    
    int vertexCount = sizeof(vertices) / sizeof(Vertex);

    // Vertex *vertices = generate_grid_vertices(rows, cols);
    // GLuint *indices = generate_indices_by_grid(rows, cols);
    GLuint vbo = create_vbo(vertices, vertexCount);
    GLuint ebo = create_ebo(indices, indexCount);
    GLuint vao = create_vao(vbo, &ebo);
    Shader standard_shader = shader_create("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    shader_use(&standard_shader);
    shader_set_int(&standard_shader, "texture1", 0);
    shader_set_int(&standard_shader, "texture2", 1);

    
    int running = 1;
    SDL_Event event;

    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
        }
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader_use(&standard_shader);
        shader_set_float(&standard_shader, "horizontal_offset", 0.5f);

        GLint location = glGetUniformLocation(standard_shader.id, "ourTexture");
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bricks_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wood_texture);
        
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}