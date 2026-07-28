#include "input.h"
#include <string.h>

static bool currentKeys[SDL_SCANCODE_COUNT];
static bool previousKeys[SDL_SCANCODE_COUNT];

void input_update_state(void) {
    memcpy(previousKeys, currentKeys, sizeof(previousKeys));
    memcpy(currentKeys, SDL_GetKeyboardState(NULL), sizeof(currentKeys));
}

bool input_key_down(SDL_Scancode key) {
    return currentKeys[key];
}

bool input_key_pressed(SDL_Scancode key) {
    return currentKeys[key] && !previousKeys[key];
}

bool input_key_released(SDL_Scancode key) {
    return !currentKeys[key] && previousKeys[key];
}

float input_axis(SDL_Scancode neg, SDL_Scancode pos) {
    return (float)currentKeys[pos] - (float)currentKeys[neg];
}
