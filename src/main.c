#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include <windows.h>
#include "graphics.h"
#include "utils.h"
#include "mesh.h"
#include "math.h"

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

    int rows = 7;
    int cols = 3;
    int vertexCount = rows * cols;
    int indexCount = (rows-1)*(cols-1)*6;

    Vertex *vertices = generate_grid_vertices(rows, cols);
    GLuint *indices = generate_indices_by_grid(rows, cols);
    GLuint vbo = create_vbo(vertices, vertexCount);
    GLuint ebo = create_ebo(indices, indexCount);
    GLuint vao = create_vao(vbo, ebo);
    GLuint shaderProgram = create_shader_program();

    
    int running = 1;
    SDL_Event event;

    Vertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {182.0f/255.0f, 97.0f/255.0f, 209.0f/255.0f, 1}},
        {{0.5f, -0.5f, 0.0f}, {151.0f/255.0f, 131.0f/255.0f, 201.0f/255.0f, 0.7}},
        {{0.0f,  0.5f, 0.0f}, {163.0f/255.0f, 116.0f/255.0f, 63.0f/255.0f, 0.4}},
        {{0.0f,  0.5f, 0.0f}, {113.0f/255.0f, 216.0f/255.0f, 13.0f/255.0f, 0.4}}
    };
    
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
        
        glUseProgram(shaderProgram);
        
        // sin(color);
        
        float timeValue = SDL_GetTicks() / 1000.0f;
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation-0.2, greenValue, greenValue+0.3, greenValue+0.6, 1.0f);
        
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