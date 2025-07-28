#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include "utils.h"

char* load_shader_source(const char* path);

GLuint create_vbo(Vertex *vertices, size_t count);
GLuint create_ebo(GLuint *indices, size_t count);
GLuint create_vao(GLuint VBO, GLuint *EBO);
void draw_elements(GLuint vao, GLuint count);

#endif