#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

#define NUM_SCANCODES 512

void input_process_event(SDL_Event *event);
bool input_is_key_down(SDL_Scancode key);
bool input_was_key_pressed(SDL_Scancode key);

#endif