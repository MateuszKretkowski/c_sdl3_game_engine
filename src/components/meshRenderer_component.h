#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "utils.h"
#include "core/component.h"

typedef struct {
    Component base;
    Mesh *mesh;
} MeshRendererComponent;

MeshRendererComponent *create_meshRenderer_component(Mesh *mesh);

#endif