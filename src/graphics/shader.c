#include <stdio.h>
#include <stdlib.h>

#include "shader.h"
#include "utils.h"
#include "string.h"

static GLuint compile_shader(const char* source_path, GLenum shader_type) {
    char* source = read_file(source_path);
    if (!source) return 0;

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char* const*)&source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "SHADER COMPILATION FAILED (%s):\n%s\n",
            shader_type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT",
            infoLog);
    }

    free(source);
    return shader;
}

Shader shader_create(const char* vertex_path, const char* fragment_path, char *name) {
    Shader shader;
    shader.id = glCreateProgram();
    shader.name = malloc(sizeof(char) * strlen(name) + 1);
    if (shader.name) {
        strcpy(shader.name, name);
    }
    else {
        printf("could not set name for shader");
    }

    GLuint vertex = compile_shader(vertex_path, GL_VERTEX_SHADER);
    GLuint fragment = compile_shader(fragment_path, GL_FRAGMENT_SHADER);

    glAttachShader(shader.id, vertex);
    glAttachShader(shader.id, fragment);
    glLinkProgram(shader.id);

    int success;
    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader.id, 512, NULL, infoLog);
        fprintf(stderr, "SHADER LINKING FAILED:\n%s\n", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

void shader_use(const Shader* shader) {
    glUseProgram(shader->id);
}

void shader_set_bool(const Shader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_int(const Shader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_float(const Shader* shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_destroy(Shader* shader) {
    if (shader && shader->id != 0) {
        glDeleteProgram(shader->id);
        shader->id = 0; // aby zaznaczyć, że program został usunięty
        free(shader->name);
    }
}