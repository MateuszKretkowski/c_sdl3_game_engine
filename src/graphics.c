#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <glad/glad.h>

void Graphics_Init() {
    GLuint vao, vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}