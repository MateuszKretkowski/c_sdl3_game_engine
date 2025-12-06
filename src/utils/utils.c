#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include "utils.h"

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = malloc(length+1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    return buffer;
}

// Vector2 functions
Vector2 vector2_add(Vector2 a, Vector2 b) {
    return (Vector2){a.x + b.x, a.y + b.y};
}

Vector2 vector2_subtract(Vector2 a, Vector2 b) {
    return (Vector2){a.x - b.x, a.y - b.y};
}

Vector2 vector2_multiply(Vector2 v, float scalar) {
    return (Vector2){v.x * scalar, v.y * scalar};
}

Vector2 vector2_divide(Vector2 v, float scalar) {
    return (Vector2){v.x / scalar, v.y / scalar};
}

float vector2_dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

float vector2_length(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2 vector2_normalize(Vector2 v) {
    float len = vector2_length(v);
    if (len > 0.0f) {
        return vector2_divide(v, len);
    }
    return v;
}

// Vector3 functions
Vector3 vector3_add(Vector3 a, Vector3 b) {
    return (Vector3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 vector3_subtract(Vector3 a, Vector3 b) {
    return (Vector3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector3 vector3_multiply(Vector3 v, float scalar) {
    return (Vector3){v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector3 vector3_divide(Vector3 v, float scalar) {
    return (Vector3){v.x / scalar, v.y / scalar, v.z / scalar};
}

float vector3_dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 vector3_cross(Vector3 a, Vector3 b) {
    return (Vector3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vector3_length(Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 vector3_normalize(Vector3 v) {
    float len = vector3_length(v);
    if (len > 0.0f) {
        return vector3_divide(v, len);
    }
    return v;
}

