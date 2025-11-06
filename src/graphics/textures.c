#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#include <glad/glad.h>

unsigned char *create_checkerboard_texture(int width, int height, int box_size) {
    unsigned char *checkerboard_texture = malloc(width * height * 3 *sizeof(unsigned char));
    if (!checkerboard_texture) {
        fprintf(stderr, "create_checkerboard_texture: failed to allocate memory for %dx%d texture\n", width, height);
        return NULL;
    }
    
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            bool isWhite = ((x/box_size)%2) ^ ((y/box_size)%2);
            int index = (y*width + x) * 3;
            int color = isWhite ? 255 : 0;
            checkerboard_texture[index+0] = color;
            checkerboard_texture[index+1] = color;
            checkerboard_texture[index+2] = color;
        }
    }
    return checkerboard_texture;
}

GLuint create_texture(const char *path) {
    int width, height, channels;

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "textures/%s", path);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *curr_tex = stbi_load(full_path, &width, &height, &channels, 4);

    bool is_checkerboard = false;
    if (!curr_tex) {
        fprintf(stderr, "create_texture: failed to load image: %s. Reason: %s\n", full_path, stbi_failure_reason());
        width = height = 64;
        curr_tex = create_checkerboard_texture(width, height, 8);
        if (!curr_tex) {
            fprintf(stderr, "create_texture: failed to create fallback checkerboard texture\n");
            return 0;
        }
        is_checkerboard = true;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, curr_tex);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (!is_checkerboard) {
        fprintf(stderr, "Texture loaded: %s (%dx%d, ID: %d)\n", path, width, height, texture);
    }

    if (is_checkerboard) {
        free(curr_tex);
    } else {
        stbi_image_free(curr_tex);
    }

    return texture;
}