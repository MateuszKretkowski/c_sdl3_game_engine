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
    char *name;
} Shader;

typedef struct PropertyValue {
    enum { PROP_FLOAT, PROP_INT, PROP_STRING } type;
    union {
        float f;
        int i;
        char *s;
    } value;
} PropertyValue;

typedef struct {
    int id;
    char *name;

    Shader shader;
    
    Texture diffuse_map;
    Texture specular_map;
    Texture normal_map;

    PropertyValue *properties;
    char **property_names;
    int property_count;
} Material;


typedef struct {
    char *name;

    GLuint vbo;
    GLuint ebo;
    GLuint vao;

    Material **materials;
    int indexCount;
} Mesh;


char *read_file(const char* filename);

#endif // UTILS_H