#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>

bool game_init(void);
void game_shutdown(void);

void game_handle_event(SDL_Event *event);
void game_update(void);

#endif