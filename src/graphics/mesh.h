#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include "utils.h"

Vertex *generate_grid_vertices(int rows, int cols);
GLuint *generate_indices_by_grid(int rows, int cols);

#endif