#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <glad/glad.h>
#include "utils.h"

char* load_shader_source(const char* path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "FILE_PATH_DOES_NOT_EXIST: %s\n", path);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *source = malloc(sizeof(char)*length+1);
    if (!source) {
        fclose(file);
        return NULL;
    }

    fread(source, 1, length, file);
    source[length] = '\0';

    fclose(file);
    return source;
}

Vertex vertices[] = {
    {{-0.5f, -0.5f, 0.0f}, {182.0f/255.0f, 97.0f/255.0f, 209.0f/255.0f, 1}},
    {{0.5f, -0.5f, 0.0f}, {151.0f/255.0f, 131.0f/255.0f, 201.0f/255.0f, 0.7}},
    {{0.0f,  0.5f, 0.0f}, {163.0f/255.0f, 116.0f/255.0f, 63.0f/255.0f, 0.4}}
};

GLuint create_vbo(Vertex vertices[], size_t count) {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*count, vertices, GL_STATIC_DRAW);
    return VBO;
}

GLuint create_vao(GLuint VBO) {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return VAO;
}

GLuint create_vertex_shader() {
    GLuint vertexShader;
    char* vertexShaderSource = load_shader_source("shaders/vertex_shader.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* src = (const GLchar*)vertexShaderSource;
    glShaderSource(vertexShader, 1, &src, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Error SHADER_VERTEX_COMPILATION_FAILED: %s\n", infoLog);
    }
    free(vertexShaderSource);
    return vertexShader;
}

GLuint create_fragment_shader() {
    GLuint fragmentShader;
    char* fragmentShaderSource = load_shader_source("shaders/fragment_shader.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* src = (const GLchar*)fragmentShaderSource;
    glShaderSource(fragmentShader, 1, &src, NULL);
    glCompileShader(fragmentShader);
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Error SHADER_FRAGMENT_COMPILATION_FAILED: %s\n", infoLog);
    }
    free(fragmentShaderSource);
    return fragmentShader;
}

GLuint create_shader_program() {
    GLuint shaderProgram;

    GLuint vertexShader = create_vertex_shader();
    GLuint fragmentShader = create_fragment_shader();

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error SHADER_PROGRAM_COMPILATION_FAILED: %s\n", infoLog);
    }
 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void draw_triangle(GLuint shaderProgram, GLuint VAO) {
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
}