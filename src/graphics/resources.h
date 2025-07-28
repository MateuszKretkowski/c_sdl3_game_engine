#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdbool.h>
#include <glad/glad.h>

// Globalne tekstury
extern GLuint tex_bricks;
extern GLuint tex_wood;

// Inicjalizuje i ładuje wszystkie zasoby (tekstury, shadery itp.)
bool resources_load(void);

// Zwalnia załadowane zasoby
void resources_unload(void);

#endif // RESOURCES_H
