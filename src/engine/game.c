#include "game.h"
#include "input.h"
#include <stdio.h>

char sample_scene[64] = "sample_scene";
char bootstrap_scene[64] = "menu_scene";

bool game_init(void) {
    load_scene(bootstrap_scene);
    return true;
}

void game_shutdown(void) {

}

void game_handle_event(SDL_Event *event) {

}

void game_update(void) {
    
}