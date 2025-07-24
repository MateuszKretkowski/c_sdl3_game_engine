#ifndef TEXTURES_H
#define TEXTURES_H

unsigned char *create_checkerboard_texture(int width, int height, int box_size);
GLuint generate_texture(int width, int height, unsigned char *data);

#endif