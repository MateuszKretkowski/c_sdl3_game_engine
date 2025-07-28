#ifndef TEXTURES_H
#define TEXTURES_H

#include "utils.h"

unsigned char *create_checkerboard_texture(int width, int height, int box_size);
GLuint generate_texture(const char *path);

#endif