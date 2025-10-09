#include <stdlib.h>
#include <stdio.h>
#include <libs/cJSON.h>
#include <string.h>
#include <utils/utils.h>

#include <graphics/textures.h>
#include <graphics/shader.h>
#include <graphics/graphics.h>
#include "json_utils.h"
#include "hashmap.h"
#include "mesh_library.h"
#include "gameObject.h"
#include "components/component_registry.h"

HashMap *material_cache;
HashMap *shader_cache;
HashMap *prefab_cache;

const char *asset_index_path = "assets/asset_index.json";
cJSON *asset_index_json;

void initialize_resource_manager() {
    material_cache = hashmap_create(hash_string, compare_string);
    shader_cache = hashmap_create(hash_string, compare_string);
    prefab_cache = hashmap_create(hash_string, compare_string);
}

Material *resource_get_material(const char* material_id) {
    if (!asset_index_json) {
        asset_index_json = open_json(asset_index_path);
        if (!asset_index_json) {
            printf("Could not cJSON_Parse() path: %s", asset_index_path);
            return NULL;
        }
    }
    if (!material_cache) {
        initialize_resource_manager();
    }
    else if (hashmap_contains(material_cache, (void*)material_id)) {
        return (Material*)hashmap_get(material_cache, (void*)material_id);
    }
    char* path = get_path_from_id(material_id, "materials");
    cJSON *material_json = open_json(path);
    if (!material_json) {
        printf("Could not cJSON_Parse() the buffer: %s\n", path);
    }
    Material *mat = malloc(sizeof(Material));
    cJSON *name = cJSON_GetObjectItemCaseSensitive(material_json, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("got name %s\n", name->valuestring);
        mat->name = strdup(name->valuestring);
    }
    cJSON *shader_id = cJSON_GetObjectItemCaseSensitive(material_json, "shader");
    if (cJSON_IsString(shader_id) && (shader_id->valuestring != NULL)) {
        printf("got shader_id %s\n", shader_id->valuestring);
        Shader shader = resource_get_shader(shader_id);
        mat->shader = shader;
    }
    
    cJSON *maps_json = cJSON_GetObjectItemCaseSensitive(material_json, "maps");
    cJSON *diffuse = cJSON_GetObjectItemCaseSensitive(maps_json, "diffuse");
    if (cJSON_IsString(diffuse) && (diffuse->valuestring != NULL)) {
        printf("got diffuse %s\n", diffuse->valuestring);
        Texture texture;
        texture.id = create_texture(diffuse->valuestring);
        texture.path = diffuse->valuestring;
        mat->diffuse_map = texture;
    }
    cJSON *specular = cJSON_GetObjectItemCaseSensitive(maps_json, "specular");
    if (cJSON_IsString(specular) && (specular->valuestring != NULL)) {
        printf("got specular %s\n", specular->valuestring);
        Texture texture;
        texture.id = create_texture(specular->valuestring);
        texture.path = specular->valuestring;
        mat->specular_map = texture;
    }
    cJSON *normal = cJSON_GetObjectItemCaseSensitive(maps_json, "normal");
    if (cJSON_IsString(normal) && (normal->valuestring != NULL)) {
        printf("got normal %s\n", normal->valuestring);
        Texture texture;
        texture.id = create_texture(normal->valuestring);
        texture.path = normal->valuestring;
        mat->normal_map = texture;
    }
    cJSON *properties = cJSON_GetObjectItemCaseSensitive(material_json, "properties");
    mat->property_count = cJSON_GetArraySize(properties);
    mat->properties = malloc(sizeof(PropertyValue) * mat->property_count);
    mat->property_names = malloc(sizeof(char*) * mat->property_count);
    int i=0;
    cJSON *property = NULL;
    cJSON_ArrayForEach(property, properties) {
        mat->property_names[i] = strdup(property->string);
        if (cJSON_IsNumber(property)) {
            double val = property->valuedouble;
            if (val == (int)val) {
                mat->properties[i].type = PROP_INT;
                mat->properties[i].value.i = (int)val;
            }
            else {
                mat->properties[i].type = PROP_FLOAT;
                mat->properties[i].value.f = (float)val;
            }
        }
        else if (cJSON_IsString(property)) {
            mat->properties[i].type = PROP_STRING;
            mat->properties[i].value.s = property->valuestring;
        }
        i++;
    }
    cJSON_Delete(material_json);

    hashmap_insert(material_cache, strdup(material_id), mat);

    return mat;
}

  Shader resource_get_shader(const char *shader_id) {
      Shader empty_shader = {0};
      if (!shader_id) {
          return empty_shader;
      }
      if (!shader_cache) {
        initialize_resource_manager();
      }
      else if (hashmap_contains(shader_cache, (void*)shader_id)) {
        Shader *cached_shader = (Shader*)hashmap_get(shader_cache, (void*)shader_id);
        return *cached_shader;
      }

      char *shader_path = get_path_from_id(shader_id, "shaders");
      cJSON *shader_json = open_json(shader_path);
      if (!shader_json) {
          return empty_shader;
      }

      cJSON *name = cJSON_GetObjectItemCaseSensitive(shader_json, "name");
      cJSON *vertex_path = cJSON_GetObjectItemCaseSensitive(shader_json, "vertex_path");
      cJSON *fragment_path = cJSON_GetObjectItemCaseSensitive(shader_json, "fragment_path");

      if (!cJSON_IsString(name) || !name->valuestring ||
          !cJSON_IsString(vertex_path) || !vertex_path->valuestring ||
          !cJSON_IsString(fragment_path) || !fragment_path->valuestring) {
          printf("ERROR: Invalid shader JSON\n");
          cJSON_Delete(shader_json);
          return empty_shader;
      }

      Shader shader = shader_create(vertex_path->valuestring, fragment_path->valuestring, name->valuestring);
      cJSON_Delete(shader_json);

      Shader *shader_copy = malloc(sizeof(Shader));
      *shader_copy = shader;
      hashmap_insert(shader_cache, strdup(shader_id), shader_copy);

      return shader;
}

Mesh *resource_get_mesh(char *mesh_id) {
    if (!mesh_id) {
        return NULL;
    }
    else if (!mesh_library) {
        initialize_mesh_library(8);
        if (!mesh_library) {
            return NULL;
        }
    }
    else if (try_get_mesh(mesh_id)) {
        return try_get_mesh(mesh_id);
    } 
    char *path = get_path_from_id(mesh_id, "meshes");
    cJSON *mesh_json = open_json(path);
    cJSON *name = cJSON_GetObjectItemCaseSensitive(mesh_json, "name");
    if (!cJSON_IsString(name) || !name->valuestring) {
        printf("ERROR: Invalid mesh JSON\n");
        cJSON_Delete(mesh_json);
        return NULL;
    }
    cJSON *vertices_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "vertices");
    int i=0;
    cJSON *iterator;
    Vertex *vertices = malloc(sizeof(Vertex) * cJSON_GetArraySize(vertices_json));
    cJSON_ArrayForEach(iterator, vertices_json) {
        vertices[i].position = parse_vector3_array(cJSON_GetObjectItemCaseSensitive(iterator, "position"));
        vertices[i].color = parse_vector4_array(cJSON_GetObjectItemCaseSensitive(iterator, "color"));
        vertices[i].uv = parse_vector2_array(cJSON_GetObjectItemCaseSensitive(iterator, "uv"));
        i++;
    }
    cJSON *indices_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "indices");
    i=0;
    GLuint *indices = malloc(sizeof(GLuint) * cJSON_GetArraySize(indices_json));
    cJSON_ArrayForEach(iterator, indices_json) {
        indices[i] = iterator->valueint;
        i++;
    }
    cJSON *materials_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "materials");
    Material **materials = malloc(sizeof(Material*) * cJSON_GetArraySize(materials_json));
    int materialCount = 0;
    cJSON_ArrayForEach(iterator, materials_json) {
        if (cJSON_IsString(iterator) && (iterator->valuestring != NULL)) {
            Material *mat = resource_get_material(iterator->valuestring);
            if (mat) {
                materials[materialCount] = mat;
                materialCount++;
            }
        }
    }
    Mesh *mesh = add_mesh_to_library(strdup(name), vertices, indices, cJSON_GetArraySize(indices_json), mesh_id, materials, materialCount);
    free(mesh);
    free(materials);
    
    return try_get_mesh(mesh_id);
}

void resource_load_prefab(char *prefab_id) {
    if (!prefab_id) {
        return;
    }
    if (!prefab_cache) {
        initialize_resource_manager();
    }
    if (hashmap_contains(prefab_cache, (void*)prefab_id)) {
        return;
    }

    char *path = get_path_from_id(prefab_id, "prefabs");
    cJSON *prefab_json = open_json(path);
    if (!prefab_json) {
        printf("Could not cJSON_Parse() the buffer: %s\n", path);
        return;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(prefab_json, "name");
    if (!cJSON_IsString(name) || !name->valuestring) {
        printf("ERROR: Invalid prefab JSON - missing name\n");
        cJSON_Delete(prefab_json);
        return;
    }

    GameObject *prefab = instantiate_gameObject(name->valuestring);

    // Parse and add components to the prefab
    cJSON *components_json = cJSON_GetObjectItemCaseSensitive(prefab_json, "components");
    if (cJSON_IsArray(components_json)) {
        cJSON *component_json = NULL;
        cJSON_ArrayForEach(component_json, components_json) {
            cJSON *component_id = cJSON_GetObjectItemCaseSensitive(component_json, "id");

            if (!cJSON_IsString(component_id) || !component_id->valuestring) {
                fprintf(stderr, "ERROR: Component missing 'id' field in prefab '%s'\n", prefab_id);
                continue;
            }

            // Create component using registry
            Component *component = component_registry_create(component_id->valuestring, component_json);
            if (component) {
                add_component(prefab, component);
                printf("Added component '%s' to prefab '%s'\n", component_id->valuestring, prefab_id);
            } else {
                fprintf(stderr, "ERROR: Failed to create component '%s' for prefab '%s'\n",
                        component_id->valuestring, prefab_id);
            }
        }
    }

    cJSON_Delete(prefab_json);

    hashmap_insert(prefab_cache, strdup(prefab_id), prefab);
}

GameObject *resource_get_prefab(char *prefab_id) {
    if (!prefab_id) {
        return NULL;
    }
    if (!prefab_cache) {
        initialize_resource_manager();
    }
    if (hashmap_contains(prefab_cache, (void*)prefab_id)) {
        return (GameObject*)hashmap_get(prefab_cache, (void*)prefab_id);
    }

    // Load the prefab if it's not in cache
    resource_load_prefab(prefab_id);

    // Return it from cache (or NULL if loading failed)
    if (hashmap_contains(prefab_cache, (void*)prefab_id)) {
        return (GameObject*)hashmap_get(prefab_cache, (void*)prefab_id);
    }

    return NULL;
}

cJSON *open_json(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Could not open the file: %s", path);
        return NULL;
    }
    char buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        printf("cannot parse buffer into json\n");
    }
    return json;
}

char *get_path_from_id(char *id, char bookmark[64]) {
    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(asset_index_json, bookmark);
    cJSON *path = cJSON_GetObjectItemCaseSensitive(id_json, id);
    return path ? path->valuestring : NULL;
}