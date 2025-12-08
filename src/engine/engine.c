#include "engine.h"
#include "window.h"
#include "game.h"
#include "input.h"
#include "resources.h"
#include "render.h"
#include "init.h"
#include "physics/physics_manager.h"
#include <glad/glad.h>

#include <SDL3/SDL.h>

static bool running = true;

bool engine_init(const char *title, int width, int height) {
    fprintf(stderr, "engine_init: Starting engine initialization\n");
    set_cwd_to_project_root();
    fprintf(stderr, "engine_init: Calling window_init\n");
    if (!window_init(title, width, height)) {
        fprintf(stderr, "engine_init: window_init failed\n");
        return false;
    }
    fprintf(stderr, "engine_init: window_init succeeded, calling resources_load\n");
    if (!resources_load()) {
        fprintf(stderr, "engine_init: resources_load failed\n");
        return false;
    }
    fprintf(stderr, "engine_init: resources_load succeeded, calling render_init\n");
    render_init();
    if (!game_init()) {
        fprintf(stderr, "engine_init: game_init failed\n");
        return false;
    }
    fprintf(stderr, "engine_init: game_init succeeded\n");
    fprintf(stderr, "engine_init: Engine initialization complete\n");
    return true;
}

void engine_run(void) {
    fprintf(stderr, "engine_run: Starting main loop\n");
    fflush(stderr);
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
        physics_manager_update();
        window_swap();
    }
    fprintf(stderr, "engine_run: Main loop ended\n");
    fflush(stderr);
}

void engine_shutdown(void) {
    game_shutdown();
    render_shutdown();
    resources_unload();
    window_shutdown();
}
