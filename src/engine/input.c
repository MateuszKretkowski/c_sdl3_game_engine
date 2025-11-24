#include "input.h"
#include <SDL3/SDL.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> // DODANO: Naprawia błąd 'fabsf'

static SDL_Gamepad *gamepad = NULL;

// Stan przycisków
static bool currentButtons[SDL_GAMEPAD_BUTTON_COUNT];
static bool previousButtons[SDL_GAMEPAD_BUTTON_COUNT];

// Stan osi
// Używamy SDL_GAMEPAD_AXIS_COUNT, by tablica miała poprawny rozmiar dla osi.
static float currentAxis[SDL_GAMEPAD_AXIS_COUNT]; 

// ------------------------------------------------------------
// Inicjalizacja
// ------------------------------------------------------------
void input_init(void) {
    int count = 0;
    SDL_JoystickID *pads = SDL_GetGamepads(&count);

    if (count > 0) {
        gamepad = SDL_OpenGamepad(pads[0]);
    }

    SDL_free(pads);

    memset(currentButtons, 0, sizeof(currentButtons));
    memset(previousButtons, 0, sizeof(previousButtons));
    memset(currentAxis, 0, sizeof(currentAxis)); 
}

// ------------------------------------------------------------
// Przetwarzanie eventów
// ------------------------------------------------------------
void input_process_event(SDL_Event *event) {

    switch (event->type) {

        case SDL_EVENT_GAMEPAD_ADDED:
            if (!gamepad) {
                gamepad = SDL_OpenGamepad(event->gdevice.which);
            }
            break;

        case SDL_EVENT_GAMEPAD_REMOVED:
            if (gamepad && SDL_GetGamepadID(gamepad) == event->gdevice.which) {
                SDL_CloseGamepad(gamepad);
                gamepad = NULL;
            }
            break;

        default:
            break;
    }
}

// ------------------------------------------------------------
// Aktualizacja stanu gamepada
// (wywołuj raz na klatkę w game loop)
// ------------------------------------------------------------
void input_update_state(void) {

    if (!gamepad) return;

    memcpy(previousButtons, currentButtons, sizeof(previousButtons));

    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
        currentButtons[i] = SDL_GetGamepadButton(gamepad, (SDL_GamepadButton)i);
    }

    for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; i++) { 
        float v = SDL_GetGamepadAxis(gamepad, (SDL_GamepadAxis)i);
        currentAxis[i] = v / 32767.0f; // normalizacja do -1..1
    }
}

// ------------------------------------------------------------
// API dla przycisków
// ------------------------------------------------------------
bool gamepad_is_button_down(SDL_GamepadButton button) {
    return currentButtons[button];
}

bool gamepad_was_button_pressed(SDL_GamepadButton button) {
    return currentButtons[button] && !previousButtons[button];
}

// ------------------------------------------------------------
// API dla osi
// ------------------------------------------------------------
float gamepad_get_axis(SDL_GamepadAxis axis) {
    return currentAxis[axis];
}

float gamepad_get_axis_deadzone(SDL_GamepadAxis axis, float deadzone) {
    float v = currentAxis[axis];
    if (fabsf(v) < deadzone) return 0.0f; 
    return v;
}