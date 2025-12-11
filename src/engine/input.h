#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL3/SDL.h>

// ─────────────────────────────────────────────────────────────
// API
// ─────────────────────────────────────────────────────────────

// Inicjalizuje system input (otwiera gamepad jeśli dostępny)
void input_init(void);

// Przetwarza eventy SDL (podłączanie/odłączanie gamepada)
void input_process_event(SDL_Event *event);

// Aktualizuje stan przycisków i osi (wołaj raz na klatkę)
void input_update_state(void);

// Zwraca true jeśli przycisk jest wciśnięty
bool input_gamepad_is_button_down(SDL_GamepadButton button);

// Zwraca true jeśli przycisk został właśnie wciśnięty (edge-detect)
bool input_gamepad_was_button_pressed(SDL_GamepadButton button);

// Pobiera oś znormalizowaną do -1..1
float input_gamepad_get_axis(SDL_GamepadAxis axis);

// Pobiera oś z deadzone: zwraca 0.0f jeśli wartość bezwzględna < deadzone
float input_gamepad_get_axis_deadzone(SDL_GamepadAxis axis, float deadzone);

#endif
