#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H

#include <utils/utils.h>

Mesh *mesh_library;
void initialize_mesh_library(int meshCount);
Mesh *try_get_mesh(char *meshName);
Mesh *add_mesh_to_library(char* meshName, Vertex *vertices, GLuint *indices, int indexCount,
                          char* meshId, Material **materials, int materialCount);
void *remove_mesh_from_library(char *meshName);
void free_mesh_library();

#endif // MESH_LIBRARY_H
