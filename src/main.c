#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "config.h"

bool isRunning;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv);

int main(int argc, char *argv[])
{
    while (isRunning) {

    }
    return 0;
}