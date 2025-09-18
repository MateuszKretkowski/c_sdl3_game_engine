#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>

typedef struct {
    GLuint id;
    char *path;
} Texture;

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
    Vector2 uv;
} Vertex;

typedef struct {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
} Transform;

typedef struct {
    GLuint id;
} Shader;

typedef struct {
    Texture diffuse_map;
    Texture specular_map;
    Texture normal_map;

    float shininess;

    Shader *shader;
} Material;

typedef struct {
    char *name;

    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    int indexCount;
} Mesh;


char *read_file(const char* filename);

#endif // UTILS_H