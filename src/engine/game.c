#include "game.h"
#include "input.h"
#include <stdio.h>

char sample_scene[64] = "sample_scene";

bool game_init(void) {
    load_scene(sample_scene);
    return true;
}

void game_shutdown(void) {

}

void game_handle_event(SDL_Event *event) {

}

void game_update(void) {
    if (input_is_key_down(SDL_SCANCODE_ESCAPE)) {
        SDL_Event quit = { .type = SDL_EVENT_QUIT };
        SDL_PushEvent(&quit);
    }
}