#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "cJSON.h"
#include <utils.h>
#include "core/gameObject.h"
#include <libs/cJSON.h>
#include <string.h>
#include <utils/utils.h>

#include <graphics/textures.h>
#include <graphics/shader.h>
#include <graphics/graphics.h>
#include "json_utils.h"
#include "hashmap.h"
#include "mesh_library.h"
#include "core/gameObject.h"
#include "components/component_registry.h"

// Forward declaration to avoid circular dependency
typedef struct Scene Scene;

void initialize_resource_manager();
Material *resource_get_material(const char* material_id);
physics_material *resource_get_physics_material(const char *physics_material_id);
Shader resource_get_shader(const char *shader_id);
Mesh *resource_get_mesh(char *mesh_id);
GameObject *resource_get_prefab(char *prefab_id);
cJSON *open_json(const char *path);
char *get_path_from_id(const char *id, const char *bookmark);
cJSON *resource_get_scene_config(char *id);
Scene *resource_get_scene(char *id);

#endif // RESOURCE_MANAGER_H