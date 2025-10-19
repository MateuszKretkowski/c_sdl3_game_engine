#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;
    Mesh *mesh;
} mesh_renderer_component;

mesh_renderer_component *create_mesh_renderer_component(Mesh *mesh);

#endif