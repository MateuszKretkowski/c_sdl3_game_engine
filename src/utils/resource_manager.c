#include <stdlib.h>
#include <stdio.h>
#include "resource_manager.h"
#include "engine/scene_manager.h"

cJSON *open_json(const char *path);
char *get_path_from_id(const char *id, const char *bookmark);
Shader resource_get_shader(const char *shader_id);

HashMap *material_cache;
HashMap *shader_cache;
HashMap *prefab_cache;

const char *asset_index_path = "src/assets/asset_index.json";
cJSON *asset_index_json;

void initialize_resource_manager()
{
    material_cache = hashmap_create(hash_string, compare_string);
    shader_cache = hashmap_create(hash_string, compare_string);
    prefab_cache = hashmap_create(hash_string, compare_string);
}

Material *resource_get_material(const char *material_id)
{
    if (!asset_index_json)
    {
        asset_index_json = open_json(asset_index_path);
        if (!asset_index_json)
        {
            fprintf(stderr, "resource_get_material: could not parse asset_index.json at path: %s\n", asset_index_path);
            return NULL;
        }
    }
    if (!material_cache)
    {
        initialize_resource_manager();
    }
    else if (hashmap_contains(material_cache, (void *)material_id))
    {
        return (Material *)hashmap_get(material_cache, (void *)material_id);
    }
    char *path = get_path_from_id(material_id, "materials");
    cJSON *material_json = open_json(path);
    if (!material_json)
    {
        fprintf(stderr, "resource_get_material: could not parse material JSON at path: %s\n", path);
        return NULL;
    }
    Material *mat = malloc(sizeof(Material));
    if (!mat) {
        fprintf(stderr, "resource_get_material: failed to allocate memory for material\n");
        cJSON_Delete(material_json);
        return NULL;
    }
    cJSON *name = cJSON_GetObjectItemCaseSensitive(material_json, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        mat->name = strdup(name->valuestring);
    }
    else
    {
        fprintf(stderr, "resource_get_material: material missing name field\n");
    }
    cJSON *shader_id = cJSON_GetObjectItemCaseSensitive(material_json, "shader");
    if (cJSON_IsString(shader_id) && (shader_id->valuestring != NULL))
    {
        Shader shader = resource_get_shader(shader_id->valuestring);
        mat->shader = shader;
    }
    else
    {
        fprintf(stderr, "resource_get_material: material missing shader field\n");
    }

    cJSON *maps_json = cJSON_GetObjectItemCaseSensitive(material_json, "maps");
    if (!maps_json) {
        fprintf(stderr, "resource_get_material: material missing maps field\n");
    }
    cJSON *diffuse = cJSON_GetObjectItemCaseSensitive(maps_json, "diffuse");
    if (cJSON_IsString(diffuse) && (diffuse->valuestring != NULL))
    {
        Texture texture;
        texture.id = create_texture(diffuse->valuestring);
        texture.path = strdup(diffuse->valuestring);
        mat->diffuse_map = texture;
    }
    cJSON *specular = cJSON_GetObjectItemCaseSensitive(maps_json, "specular");
    if (cJSON_IsString(specular) && (specular->valuestring != NULL))
    {
        Texture texture;
        texture.id = create_texture(specular->valuestring);
        texture.path = strdup(specular->valuestring);
        mat->specular_map = texture;
    }
    cJSON *normal = cJSON_GetObjectItemCaseSensitive(maps_json, "normal");
    if (cJSON_IsString(normal) && (normal->valuestring != NULL))
    {
        Texture texture;
        texture.id = create_texture(normal->valuestring);
        texture.path = strdup(normal->valuestring);
        mat->normal_map = texture;
    }
    cJSON *properties = cJSON_GetObjectItemCaseSensitive(material_json, "properties");
    mat->property_count = cJSON_GetArraySize(properties);
    mat->properties = malloc(sizeof(PropertyValue) * mat->property_count);
    mat->property_names = malloc(sizeof(char *) * mat->property_count);
    int i = 0;
    cJSON *property = NULL;
    cJSON_ArrayForEach(property, properties)
    {
        mat->property_names[i] = strdup(property->string);
        if (cJSON_IsNumber(property))
        {
            double val = property->valuedouble;
            if (val == (int)val)
            {
                mat->properties[i].type = PROP_INT;
                mat->properties[i].value.i = (int)val;
            }
            else
            {
                mat->properties[i].type = PROP_FLOAT;
                mat->properties[i].value.f = (float)val;
            }
        }
        else if (cJSON_IsString(property))
        {
            mat->properties[i].type = PROP_STRING;
            mat->properties[i].value.s = property->valuestring;
        }
        i++;
    }
    cJSON_Delete(material_json);

    hashmap_insert(material_cache, strdup(material_id), mat);

    return mat;
}

Shader resource_get_shader(const char *shader_id)
{
    Shader empty_shader = {0};
    if (!shader_id)
    {
        fprintf(stderr, "resource_get_shader: shader_id is NULL\n");
        return empty_shader;
    }
    if (!shader_cache)
    {
        initialize_resource_manager();
    }
    else if (hashmap_contains(shader_cache, (void *)shader_id))
    {
        Shader *cached_shader = (Shader *)hashmap_get(shader_cache, (void *)shader_id);
        return *cached_shader;
    }

    char *shader_path = get_path_from_id(shader_id, "shaders");
    if (!shader_path) {
        fprintf(stderr, "resource_get_shader: could not get path for shader_id: %s\n", shader_id);
        return empty_shader;
    }
    cJSON *shader_json = open_json(shader_path);
    if (!shader_json)
    {
        fprintf(stderr, "resource_get_shader: could not open shader JSON at path: %s\n", shader_path);
        return empty_shader;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(shader_json, "name");
    cJSON *vertex_path = cJSON_GetObjectItemCaseSensitive(shader_json, "vertex_path");
    cJSON *fragment_path = cJSON_GetObjectItemCaseSensitive(shader_json, "fragment_path");

    if (!cJSON_IsString(name) || !name->valuestring ||
        !cJSON_IsString(vertex_path) || !vertex_path->valuestring ||
        !cJSON_IsString(fragment_path) || !fragment_path->valuestring)
    {
        fprintf(stderr, "resource_get_shader: invalid shader JSON for shader_id: %s\n", shader_id);
        cJSON_Delete(shader_json);
        return empty_shader;
    }

    Shader shader = shader_create(vertex_path->valuestring, fragment_path->valuestring, name->valuestring);
    cJSON_Delete(shader_json);

    Shader *shader_copy = malloc(sizeof(Shader));
    if (!shader_copy) {
        fprintf(stderr, "resource_get_shader: failed to allocate memory for shader copy\n");
        return shader;
    }
    *shader_copy = shader;
    hashmap_insert(shader_cache, strdup(shader_id), shader_copy);

    return shader;
}

Scene *resource_get_scene(char *id) {
    if (!id) {
        fprintf(stderr, "resource_get_scene: id is NULL\n");
        return NULL;
    }
    char *scene_path = get_path_from_id(id, "scenes");
    if (!scene_path) {
        fprintf(stderr, "resource_get_scene: could not get path for scene_id: %s\n", id);
        return NULL;
    }
    cJSON *scene_json = open_json(scene_path);
    if (!scene_json)
    {
        fprintf(stderr, "resource_get_scene: cannot read scene_json for scene_id: %s\n", id);
        return NULL;
    }
    cJSON *gameObjects_json = cJSON_GetObjectItemCaseSensitive(scene_json, "gameObjects");
    if (!gameObjects_json) {
        fprintf(stderr, "resource_get_scene: no gameObjects in scene: %s\n", id);
        cJSON_Delete(scene_json);
        return NULL;
    }

    int gameObjects_count = cJSON_GetArraySize(gameObjects_json);
    Scene *scene = malloc(sizeof(Scene));
    if (!scene) {
        fprintf(stderr, "resource_get_scene: failed to allocate memory for scene\n");
        cJSON_Delete(scene_json);
        return NULL;
    }
    scene->id = strdup(id);
    scene->gameObjects_length = gameObjects_count;
    scene->gameObjects = malloc(sizeof(GameObject) * gameObjects_count);
    if (!scene->gameObjects) {
        fprintf(stderr, "resource_get_scene: failed to allocate memory for gameObjects array\n");
        free(scene);
        cJSON_Delete(scene_json);
        return NULL;
    }

    // Parse each GameObject from JSON
    int i = 0;
    cJSON *gameObject_json = NULL;
    cJSON_ArrayForEach(gameObject_json, gameObjects_json) {
        cJSON *name_json = cJSON_GetObjectItemCaseSensitive(gameObject_json, "name");
        const char *name_const = (name_json && cJSON_IsString(name_json)) ? name_json->valuestring : "GameObject";
        char *name = strdup(name_const);

        GameObject *go = instantiate_gameObject(name);
        if (go) {
            fprintf(stderr, "resource_get_scene: Created GameObject - name: %s, id: %s, components: %d\n",
                    go->name ? go->name : "NULL",
                    go->id ? go->id : "NULL",
                    go->components_length);
            // Copy the GameObject data into the scene array
            scene->gameObjects[i] = *go;
            free(go); // Free the original allocation since we copied it
            i++;
        }
        else {
            fprintf(stderr, "resource_get_scene: failed to instantiate gameObject with name: %s\n", name);
        }
        free(name);
    }

    cJSON_Delete(scene_json);
    return scene;
}

Mesh *resource_get_mesh(char *mesh_id)
{
    if (!mesh_id)
    {
        fprintf(stderr, "resource_get_mesh: mesh_id is NULL\n");
        return NULL;
    }
    else if (!mesh_library)
    {
        initialize_mesh_library(8);
        if (!mesh_library)
        {
            fprintf(stderr, "resource_get_mesh: failed to initialize mesh_library\n");
            return NULL;
        }
    }
    else if (try_get_mesh(mesh_id))
    {
        return try_get_mesh(mesh_id);
    }
    char *path = get_path_from_id(mesh_id, "meshes");
    if (!path) {
        fprintf(stderr, "resource_get_mesh: could not get path for mesh_id: %s\n", mesh_id);
        return NULL;
    }
    cJSON *mesh_json = open_json(path);
    if (!mesh_json) {
        fprintf(stderr, "resource_get_mesh: could not open mesh JSON at path: %s\n", path);
        return NULL;
    }
    cJSON *name = cJSON_GetObjectItemCaseSensitive(mesh_json, "name");
    if (!cJSON_IsString(name) || !name->valuestring)
    {
        fprintf(stderr, "resource_get_mesh: invalid mesh JSON for mesh_id: %s\n", mesh_id);
        cJSON_Delete(mesh_json);
        return NULL;
    }
    cJSON *vertices_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "vertices");
    int i = 0;
    cJSON *iterator;
    Vertex *vertices = malloc(sizeof(Vertex) * cJSON_GetArraySize(vertices_json));
    cJSON_ArrayForEach(iterator, vertices_json)
    {
        vertices[i].position = parse_vector3_array(cJSON_GetObjectItemCaseSensitive(iterator, "position"));
        vertices[i].color = parse_vector4_array(cJSON_GetObjectItemCaseSensitive(iterator, "color"));
        vertices[i].uv = parse_vector2_array(cJSON_GetObjectItemCaseSensitive(iterator, "uv"));
        i++;
    }
    cJSON *indices_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "indices");
    i = 0;
    GLuint *indices = malloc(sizeof(GLuint) * cJSON_GetArraySize(indices_json));
    cJSON_ArrayForEach(iterator, indices_json)
    {
        indices[i] = iterator->valueint;
        i++;
    }
    cJSON *materials_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "materials");
    Material **materials = malloc(sizeof(Material *) * cJSON_GetArraySize(materials_json));
    int materialCount = 0;
    cJSON_ArrayForEach(iterator, materials_json)
    {
        if (cJSON_IsString(iterator) && (iterator->valuestring != NULL))
        {
            Material *mat = resource_get_material(iterator->valuestring);
            if (mat)
            {
                materials[materialCount] = mat;
                materialCount++;
            }
        }
    }
    Mesh *mesh = add_mesh_to_library(strdup(name->valuestring), vertices, indices, cJSON_GetArraySize(indices_json), mesh_id, materials, materialCount);
    free(mesh);
    free(materials);

    return try_get_mesh(mesh_id);
}

void resource_load_prefab(char *prefab_id)
{
    if (!prefab_id)
    {
        fprintf(stderr, "resource_load_prefab: prefab_id is NULL\n");
        return;
    }
    if (!prefab_cache)
    {
        initialize_resource_manager();
    }
    if (hashmap_contains(prefab_cache, (void *)prefab_id))
    {
        return;
    }

    char *path = get_path_from_id(prefab_id, "prefabs");
    if (!path) {
        fprintf(stderr, "resource_load_prefab: could not get path for prefab_id: %s\n", prefab_id);
        return;
    }
    cJSON *prefab_json = open_json(path);
    if (!prefab_json)
    {
        fprintf(stderr, "resource_load_prefab: could not parse prefab JSON at path: %s\n", path);
        return;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(prefab_json, "name");
    if (!cJSON_IsString(name) || !name->valuestring)
    {
        fprintf(stderr, "resource_load_prefab: invalid prefab JSON - missing name for prefab_id: %s\n", prefab_id);
        cJSON_Delete(prefab_json);
        return;
    }

    GameObject *prefab = instantiate_gameObject(name->valuestring);
    if (!prefab) {
        fprintf(stderr, "resource_load_prefab: failed to instantiate gameObject for prefab: %s\n", prefab_id);
        cJSON_Delete(prefab_json);
        return;
    }

    cJSON *components_json = cJSON_GetObjectItemCaseSensitive(prefab_json, "components");
    if (cJSON_IsArray(components_json))
    {
        cJSON *component_json = NULL;
        cJSON_ArrayForEach(component_json, components_json)
        {
            cJSON *component_id = cJSON_GetObjectItemCaseSensitive(component_json, "id");

            if (!cJSON_IsString(component_id) || !component_id->valuestring)
            {
                fprintf(stderr, "ERROR: Component missing 'id' field in prefab '%s'\n", prefab_id);
                continue;
            }
            Component *component = component_registry_create(component_id->valuestring, component_json);
            if (component)
            {
                add_component(prefab, component);
            }
            else
            {
                fprintf(stderr, "ERROR: Failed to create component '%s' for prefab '%s'\n",
                        component_id->valuestring, prefab_id);
            }
        }
    }

    cJSON_Delete(prefab_json);

    fprintf(stderr, "resource_load_prefab: Loaded prefab - id: %s, name: %s, components: %d\n",
            prefab_id,
            prefab->name ? prefab->name : "NULL",
            prefab->components_length);

    hashmap_insert(prefab_cache, strdup(prefab_id), prefab);
}

GameObject *resource_get_prefab(char *prefab_id)
{
    if (!prefab_id)
    {
        fprintf(stderr, "resource_get_prefab: prefab_id is NULL\n");
        return NULL;
    }
    if (!prefab_cache)
    {
        initialize_resource_manager();
    }
    if (hashmap_contains(prefab_cache, (void *)prefab_id))
    {
        GameObject *prefab = (GameObject *)hashmap_get(prefab_cache, (void *)prefab_id);
        fprintf(stderr, "resource_get_prefab: Retrieved from cache - id: %s, name: %s, components: %d\n",
                prefab_id,
                prefab->name ? prefab->name : "NULL",
                prefab->components_length);
        return prefab;
    }

    // Load the prefab if it's not in cache
    resource_load_prefab(prefab_id);

    // Return it from cache (or NULL if loading failed)
    if (hashmap_contains(prefab_cache, (void *)prefab_id))
    {
        GameObject *prefab = (GameObject *)hashmap_get(prefab_cache, (void *)prefab_id);
        fprintf(stderr, "resource_get_prefab: Retrieved after loading - id: %s, name: %s, components: %d\n",
                prefab_id,
                prefab->name ? prefab->name : "NULL",
                prefab->components_length);
        return prefab;
    }

    fprintf(stderr, "resource_get_prefab: failed to load prefab: %s\n", prefab_id);
    return NULL;
}

cJSON *open_json(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        fprintf(stderr, "open_json: could not open file: %s\n", path);
        return NULL;
    }
    char buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    cJSON *json = cJSON_Parse(buffer);
    if (!json)
    {
        fprintf(stderr, "open_json: cannot parse buffer into JSON from file: %s\n", path);
    }
    return json;
}

char *get_path_from_id(const char *id, const char *bookmark)
{
    if (!asset_index_json)
    {
        asset_index_json = open_json(asset_index_path);
        if (!asset_index_json)
        {
            fprintf(stderr, "get_path_from_id: could not load asset_index.json from path: %s\n", asset_index_path);
            return NULL;
        }
    }
    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(asset_index_json, bookmark);
    if (!id_json)
    {
        fprintf(stderr, "get_path_from_id: could not find bookmark '%s' in asset_index.json\n", bookmark);
        return NULL;
    }
    cJSON *path = cJSON_GetObjectItemCaseSensitive(id_json, id);
    if (!path)
    {
        fprintf(stderr, "get_path_from_id: could not find id '%s' under bookmark '%s' in asset_index.json\n", id, bookmark);
        return NULL;
    }
    return path->valuestring;
}