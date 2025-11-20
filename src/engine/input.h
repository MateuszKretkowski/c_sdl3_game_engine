#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL3/SDL.h>

// ─────────────────────────────────────────────────────────────
// ENUM: Stany przycisków
// ─────────────────────────────────────────────────────────────
typedef enum {
    BUTTON_RELEASED,   // nie wciśnięty
    BUTTON_PRESSED,    // właśnie wciśnięty
    BUTTON_HELD,       // trzymany
    BUTTON_RELEASED_NOW // właśnie puszczony
} ButtonState;

// ─────────────────────────────────────────────────────────────
// Struktura gamepada
// ─────────────────────────────────────────────────────────────
typedef struct {
    SDL_Gamepad *handle;

    ButtonState buttons[SDL_GAMEPAD_BUTTON_COUNT];
    float axes[SDL_GAMEPAD_AXIS_COUNT];

    float deadzone; // np. 0.2f
} Gamepad;

// ─────────────────────────────────────────────────────────────
// API
// ─────────────────────────────────────────────────────────────

// Inicjalizuje gamepada — zwraca true jeśli znaleziono
bool Gamepad_Init(Gamepad *pad, float deadzone);

// Aktualizuje wszystkie przyciski i osie (wołasz raz na klatkę)
void Gamepad_Update(Gamepad *pad);

// Zwraca true jeśli przycisk jest wciśnięty (Pressed lub Held)
bool Gamepad_IsDown(Gamepad *pad, SDL_GamepadButton btn);

// Zwraca true jeśli przycisk został właśnie wciśnięty
bool Gamepad_Pressed(Gamepad *pad, SDL_GamepadButton btn);

// Zwraca true jeśli przycisk został właśnie puszczony
bool Gamepad_Released(Gamepad *pad, SDL_GamepadButton btn);

// Pobiera oś z deadzone: zwraca -1..1
float Gamepad_GetAxis(Gamepad *pad, SDL_GamepadAxis axis);

#endif
