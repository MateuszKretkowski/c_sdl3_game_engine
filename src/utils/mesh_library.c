#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "utils.h"
#include "graphics/graphics.h"

Mesh *mesh_library;
int count;
int capacity;

void initialize_mesh_library(int meshCount) {
    mesh_library = malloc(sizeof(Mesh) * meshCount * 2);
    capacity = meshCount * 2;
    count = 0;
    if (!mesh_library) {
        fprintf(stderr, "Failed to allocate memory for mesh library\n");
        exit(EXIT_FAILURE);
    }
}

void free_mesh_library() {
    free(mesh_library);
}

Mesh *try_get_mesh(char *meshName) {
    if (!mesh_library) {
        fprintf(stderr, "Mesh library not initialized\n");
        return NULL;
    }

    for (int i=0; i<=count-1; i++) {
        if (strcmp(mesh_library[i].name, meshName) == 0) {
            fprintf(stderr, "Found mesh %s in library\n", meshName);
            return &mesh_library[i];
        }
    }
    fprintf(stderr, "Mesh %s not found in library\n", meshName);

    return NULL;
}

Mesh *add_mesh_to_library(char* meshName, Vertex *vertices, GLuint *indices, int indexCount) {
    if (!mesh_library) {
        fprintf(stderr, "Mesh library not initialized\n");
        return NULL;
    }
    
    if (count == capacity) {
        capacity *= 2;
        mesh_library = realloc(mesh_library, capacity);
    }

    GLuint vbo = create_vbo(vertices, indexCount);
    GLuint ebo = create_ebo(indices, indexCount);
    GLuint vao = create_vao(vbo, ebo);

    Mesh newMesh;
    newMesh.name = strdup(meshName);
    newMesh.VBO = vbo;
    newMesh.EBO = ebo;
    newMesh.VAO = vao;
    newMesh.indexCount = indexCount;

    mesh_library[count] = newMesh;

    count++;

    return &mesh_library[count];
}

Mesh *remove_mesh_from_library(char *meshName) {
    if (!mesh_library) {
        fprintf(stderr, "Mesh library not initialized\n");
        return NULL;
    }
    
    Mesh removed_mesh;

    for (int i=0; i<=count-1; i++) {
        if (strcmp(mesh_library[i].name, meshName) == 0) {
            free(mesh_library[i].name);
            mesh_library[i] = mesh_library[count-1];
            count--;
        }
    }

    return &removed_mesh;
}