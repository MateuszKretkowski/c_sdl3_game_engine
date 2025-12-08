#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include "stdbool.h"

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
    char *id;
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
    char* id;
    char *name;

    GLuint vbo;
    GLuint ebo;
    GLuint vao;

    Material **materials;
    int indexCount;
} Mesh;

typedef struct {
    char *id;
    char *name;
    float friction;
    float restitution;
    float density;
} physics_material;

char *read_file(const char* filename);

// Vector2 functions
Vector2 vector2_zero();
Vector2 vector2_add(Vector2 a, Vector2 b);
Vector2 vector2_subtract(Vector2 a, Vector2 b);
Vector2 vector2_multiply(Vector2 v, float scalar);
Vector2 vector2_divide(Vector2 v, float scalar);
float vector2_dot(Vector2 a, Vector2 b);
float vector2_length(Vector2 v);
Vector2 vector2_normalize(Vector2 v);

// Vector3 functions
Vector3 vector3_zero();
Vector3 vector3_add(Vector3 a, Vector3 b);
Vector3 vector3_subtract(Vector3 a, Vector3 b);
Vector3 vector3_multiply(Vector3 v, float scalar);
Vector3 vector3_divide(Vector3 v, float scalar);
float vector3_dot(Vector3 a, Vector3 b);
Vector3 vector3_cross(Vector3 a, Vector3 b);
float vector3_length(Vector3 v);
Vector3 vector3_normalize(Vector3 v);

bool vector3_compare(Vector3 a, Vector3 b);

// Conversion functions
Vector3 vector2_to_vector3(Vector2 v, float z);
Vector2 vector3_to_vector2(Vector3 v);

float max(float a, float b);
float min(float a, float b);

#endif // UTILS_H