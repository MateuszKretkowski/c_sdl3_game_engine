#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <glad/glad.h>
#include "utils.h"

GLuint create_vbo(Vertex *vertices, size_t count) {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*count, vertices, GL_STATIC_DRAW);
    return VBO;
}

GLuint create_vao(GLuint VBO, GLuint *EBO) {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (EBO != NULL) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    }
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return VAO;
}

GLuint create_ebo(GLuint *indices, size_t count) {
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), indices, GL_STATIC_DRAW);
    return EBO;
}

void draw_elements(GLuint vao, GLuint count) {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}