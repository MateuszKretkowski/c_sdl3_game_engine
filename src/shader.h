#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct {
    GLuint id;
} Shader;

Shader shader_create(const char* vertex_path, const char* fragment_path);
void shader_use(const Shader* shader);
void shader_set_bool(const Shader* shader, const char* name, int value);
void shader_set_int(const Shader* shader, const char* name, int value);
void shader_set_float(const Shader* shader, const char* name, float value);

#endif