#include <SDL3/SDL.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"

static const _Bool *currentKeys = NULL;
static _Bool previousKeys[NUM_SCANCODES];

void input_process_event(SDL_Event *event) {
    // future stuff
}

void input_update_state(void) {
    if (!currentKeys) {
        currentKeys = SDL_GetKeyboardState(NULL);
        memset(previousKeys, 0, NUM_SCANCODES);  // zeruj na starcie
        return;
    }

    memcpy(previousKeys, currentKeys, NUM_SCANCODES);
    currentKeys = SDL_GetKeyboardState(NULL);
}

bool input_is_key_down(SDL_Scancode key) {
    return currentKeys && currentKeys[key];
}

bool input_was_key_pressed(SDL_Scancode key) {
    return currentKeys && previousKeys && currentKeys[key] && !previousKeys[key];
}
