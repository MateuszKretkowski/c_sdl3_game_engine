#pragma once
#include <stdbool.h>
#include <SDL3/SDL.h>

// Inicjalizuje SDL, okno i kontekst OpenGL
bool window_init(const char *title, int width, int height);

// Zamyka i zwalnia zasoby
void window_shutdown(void);

// Zwraca wskaźnik do głównego okna SDL (jeśli potrzebny np. do ustawień GUI)
SDL_Window *window_get(void);

// Zwraca rozmiar okna
void window_get_size(int *width, int *height);

// Wywołuje SDL_GL_SwapWindow
void window_swap(void);
