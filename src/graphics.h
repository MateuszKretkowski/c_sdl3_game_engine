#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include "utils.h"

char* load_shader_source;

GLuint create_vbo(Vector3 vertices[], size_t count);
GLuint create_vao(GLuint VBO);
GLuint create_vertex_shader(void);
GLuint create_fragment_shader(void);
GLuint create_shader_program(void);
void draw_triangle(GLuint shaderProgram, GLuint VAO);

#endif