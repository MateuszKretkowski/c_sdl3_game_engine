#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL3/SDL.h>

// ─────────────────────────────────────────────────────────────
// API
// ─────────────────────────────────────────────────────────────

// Aktualizuje stan klawiatury (wołaj raz na klatkę, po SDL_PollEvent)
void input_update_state(void);

// Zwraca true jeśli klawisz jest trzymany
bool input_key_down(SDL_Scancode key);

// Zwraca true jeśli klawisz został wciśnięty w tej klatce (edge-detect)
bool input_key_pressed(SDL_Scancode key);

// Zwraca true jeśli klawisz został puszczony w tej klatce (edge-detect)
bool input_key_released(SDL_Scancode key);

// Para klawiszy jako oś: -1.0 dla neg, 1.0 dla pos, 0.0 dla obu/żadnego
float input_axis(SDL_Scancode neg, SDL_Scancode pos);

#endif
