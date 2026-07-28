#include "game.h"
#include "input.h"
#include <stdio.h>

char sample_scene[64] = "sample_scene";
char pong_scene[64] = "pong_scene";

bool game_init(void) {
    load_scene(pong_scene);
    return true;
}

void game_shutdown(void) {

}

void game_handle_event(SDL_Event *event) {

}

void game_update(void) {
    
}