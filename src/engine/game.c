#include "game.h"
#include "input.h"
#include <stdio.h>

bool game_init(void) {
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