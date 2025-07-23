#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vector4;

typedef struct {
    Vector3 position;
    Vector4 color;
} Vertex;

typedef struct {
    GLuint id;
} Shader;

char *read_file(const char* filename);

#endif // UTILS_H