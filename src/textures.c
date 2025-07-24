#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <glad/glad.h>

unsigned char *create_checkerboard_texture(int width, int height, int box_size) {
    unsigned char *checkerboard_texture = malloc(width * height * 3 *sizeof(unsigned char));
    
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

GLuint generate_texture(int width, int height, unsigned char *data) {
    unsigned char *curr_tex = data;
    bool shouldFree = false;
    if (curr_tex == NULL) {
        curr_tex = create_checkerboard_texture(width, height, 20);
        shouldFree = true;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (curr_tex) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, curr_tex);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        fprintf(stderr, "ERROR: FAILED TO LOAD TEXTURE");
    }
    if (shouldFree) {
        free(curr_tex);
    }
    return texture;
}