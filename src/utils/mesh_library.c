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

void DeleteBuffers(int i) {
    glDeleteBuffers(1, &mesh_library[i].vbo);
    glDeleteBuffers(1, &mesh_library[i].ebo);
    glDeleteVertexArrays(1, &mesh_library[i].vao);
}

Mesh *try_get_mesh(char *mesh_id) {
    if (!mesh_library) {
        fprintf(stderr, "Mesh library not initialized\n");
        return NULL;
    }
    
    for (int i=0; i<=count-1; i++) {
        if (strcmp(mesh_library[i].id, mesh_id) == 0) {
            fprintf(stderr, "Found mesh %s in library\n", mesh_id);
            return &mesh_library[i];
        }
    }
    fprintf(stderr, "Mesh %s not found in library\n", mesh_id);
    
    return NULL;
}

Mesh *add_mesh_to_library(char* meshName, Vertex *vertices, GLuint *indices, int indexCount,
                          char* meshId, Material **materials, int materialCount) {
    if (!mesh_library) {
        fprintf(stderr, "Mesh library not initialized\n");
        initialize_mesh_library(8);
        if (!mesh_library) {
            return NULL;
        }
    }

    if (count == capacity) {
        capacity *= 2;
        mesh_library = realloc(mesh_library, capacity);
    }

    GLuint vbo = create_vbo(vertices, indexCount);
    GLuint ebo = create_ebo(indices, indexCount);
    GLuint vao = create_vao(vbo, &ebo);

    Mesh newMesh;
    newMesh.id = meshId ? strdup(meshId) : strdup(meshName);
    newMesh.name = strdup(meshName);
    newMesh.vbo = vbo;
    newMesh.ebo = ebo;
    newMesh.vao = vao;
    newMesh.indexCount = indexCount;

    if (materials && materialCount > 0) {
        newMesh.materials = malloc(sizeof(Material*) * materialCount);
        for (int i = 0; i < materialCount; i++) {
            newMesh.materials[i] = materials[i];
        }
    } else {
        newMesh.materials = NULL;
    }

    mesh_library[count] = newMesh;

    count++;

    return &mesh_library[count-1];
}

void *remove_mesh_from_library(char *meshName) {
    if (!mesh_library) {
        fprintf(stderr, "Mesh library not initialized\n");
        return NULL;
    }

    for (int i=0; i<=count-1; i++) {
        if (strcmp(mesh_library[i].name, meshName) == 0) {
            DeleteBuffers(i);
            free(mesh_library[i].name);
            if (mesh_library[i].id) {
                free(mesh_library[i].id);
            }
            if (mesh_library[i].materials) {
                free(mesh_library[i].materials);
            }
            mesh_library[i] = mesh_library[count-1];
            count--;
        }
    }
    return NULL;
}


void free_mesh_library() {
    for (int i=0; i<count; i++) {
        DeleteBuffers(i);
        free(mesh_library[i].name);
        if (mesh_library[i].id) {
            free(mesh_library[i].id);
        }
        if (mesh_library[i].materials) {
            free(mesh_library[i].materials);  // Zwalnia tablicę wskaźników (nie same materiały!)
        }
    }
    free(mesh_library);
}