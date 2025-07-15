#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <glad/glad.h>
#include "utils.h"

Vertex *generate_grid_vertices(int rows, int cols) {
    Vertex *vertices = malloc((rows)*(cols)*sizeof(Vertex));
    float scale = 0.5f;
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            int i = row * cols + col;

            vertices[i].position = (Vector3){
                col/10.0f,
                row/10.0f,
                0.0f
            };

            vertices[i].color = (Vector4){
                1.0f/256.0f,
                123.0f/256.0f,
                185.0f/256.0f,
                1.0f
            };
        }
    }

    return vertices;
}


GLuint *generate_indices_by_grid(int rows, int cols) {
    GLuint *indices = malloc((rows-1)*(cols-1)*6*sizeof(GLuint));
    int index = 0;
    for (int row=0; row<rows-1; row++) {
        for (int col=0; col<cols-1; col++) {
            int upperLeft = row*cols + col;
            int upperRight = row*cols + col+1;
            int lowerLeft = (row+1)*cols + col;
            int lowerRight = (row+1)*cols + col+1;

            indices[index++] = upperLeft;
            indices[index++] = upperRight;
            indices[index++] = lowerLeft;
            indices[index++] = upperRight;
            indices[index++] = lowerRight;
            indices[index++] = lowerLeft;
        }
    }
    return indices;
}