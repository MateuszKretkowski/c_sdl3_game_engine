#include "engine.h"
#include "window.h"
#include "game.h"
#include "input.h"
#include "resources.h"
#include "render.h"
#include "init.h"

#include <glad/glad.h>

#include <SDL3/SDL.h>

static bool running = true;

bool engine_init(const char *title, int width, int height) {
    set_cwd_to_project_root();
    if (!window_init(title, width, height)) {
        fprintf(stderr, "engine_init: window_init failed\n");
        return false;
    }
    if (!resources_load()) {
        fprintf(stderr, "engine_init: resources_load failed\n");
        return false;
    }
    if (!game_init()) {
        fprintf(stderr, "engine_init: game_init failed\n");
        return false;
    }
    render_init();
    return true;
}

void engine_run(void) {
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            } else {
                input_process_event(&event);
                game_handle_event(&event);
            }
        }

        game_update();
        render_frame();
        window_swap();
    }
}

void engine_shutdown(void) {
    game_shutdown();
    render_shutdown();
    resources_unload();
    window_shutdown();
}
