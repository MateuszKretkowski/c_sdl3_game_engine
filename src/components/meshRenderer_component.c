#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meshRenderer_component.h"
#include "utils/resource_manager.h"
#include "component_registry.h"
#include "json_utils.h"
#include "graphics/render.h"
#include <cglm/cglm.h>

void meshRenderer_awake(Component* self) {
    mesh_renderer_component *t = (mesh_renderer_component*)self;
}

void meshRenderer_start(Component* self) {
    mesh_renderer_component *t = (mesh_renderer_component*)self;
}

void meshRenderer_update(Component* self) {
    mesh_renderer_component *t = (mesh_renderer_component*)self;

}

void meshRenderer_destroy(Component* self) {
    mesh_renderer_component *t = (mesh_renderer_component*)self;

}

mesh_renderer_component *create_mesh_renderer_component(Mesh *mesh) {
    mesh_renderer_component* t = malloc(sizeof(mesh_renderer_component));
    t->base.id = "meshrenderer_component";
    t->base.name = "MeshRenderer";
    t->base.isActive = true;
    t->base.gameObject = NULL;
    t->base.standard_voids = malloc(sizeof(component_standard_voids));
    t->base.standard_voids->awake = meshRenderer_awake;
    t->base.standard_voids->start = meshRenderer_start;
    t->base.standard_voids->update = meshRenderer_update;
    t->base.standard_voids->destroy = meshRenderer_destroy;

    // variables:
    t->mesh = mesh;
    return t;
}

Component* meshRenderer_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: meshrenderer_from_json received NULL json\n");
        return NULL;
    }

    Mesh *mesh = NULL;

    cJSON *mesh_json = cJSON_GetObjectItemCaseSensitive(json, "mesh");
    if (mesh_json) {
        mesh = resource_get_mesh(mesh_json->valuestring);
    }

    return (Component*)create_mesh_renderer_component(mesh);
}

__attribute__((constructor))
static void register_meshrenderer_component() {
    component_registry_register("meshrenderer_component", meshRenderer_from_json);
}