#include "window.h"
#include <glad/glad.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_GLContext gl_context = NULL;

bool window_init(const char *title, int width, int height) {
    fprintf(stderr, "=== Window Initialization ===\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 1) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return false;
    }

    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to load OpenGL functions with GLAD\n");
        return false;
    }

    // Log OpenGL info
    fprintf(stderr, "OpenGL Version: %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    fprintf(stderr, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    fprintf(stderr, "Window: %dx%d (VSync: ON)\n", width, height);

    SDL_GL_SetSwapInterval(1); // VSync ON

    // Ustawienie viewportu na rozmiar okna
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    return true;
}

void window_shutdown(void) {
    if (gl_context) SDL_GL_DestroyContext(gl_context);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Window *window_get(void) {
    return window;
}

void window_get_size(int *width, int *height) {
    if (window) {
        SDL_GetWindowSize(window, width, height);
    } else {
        *width = 0;
        *height = 0;
    }
}

void window_swap(void) {
    SDL_GL_SwapWindow(window);
}
