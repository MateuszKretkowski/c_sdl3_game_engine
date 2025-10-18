#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "cJSON.h"
#include <utils.h>
#include "core/gameObject.h"

void initialize_resource_manager();
Material *resource_get_material(const char* material_id);
Shader resource_get_shader(const char *shader_id);
Mesh *resource_get_mesh(const char *mesh_id);
GameObject *resource_get_prefab(char *prefab_id);
cJSON *open_json(const char *path);
char *get_path_from_id(char *id, char bookmark[64]);


#endif // RESOURCE_MANAGER_H
