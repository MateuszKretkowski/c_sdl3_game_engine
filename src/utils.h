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

#endif // UTILS_H