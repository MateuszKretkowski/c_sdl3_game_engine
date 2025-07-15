#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include "utils.h"

char* load_shader_source(const char* path);

GLuint create_vbo(Vertex *vertices, size_t count);
GLuint create_ebo(GLuint *indices, size_t count);
GLuint create_vao(GLuint VBO, GLuint EBO);
GLuint create_vertex_shader(void);
GLuint create_fragment_shader(void);
GLuint create_shader_program(void);
void draw_triangle(GLuint shaderProgram, GLuint VAO, int indexCount);

#endif