#include <stdlib.h>
#include <stdio.h>
#include "resource_manager.h"
#include "engine/scene_manager.h"
#include "components/transform_component.h"

cJSON *open_json(const char *path);
char *get_path_from_id(const char *id, const char *bookmark);
Shader resource_get_shader(const char *shader_id);

HashMap *material_cache = NULL;
HashMap *shader_cache = NULL;
HashMap *prefab_cache = NULL;

const char *asset_index_path = "src/assets/asset_index.json";
cJSON *asset_index_json = NULL;

void initialize_resource_manager()
{
    fprintf(stderr, "initialize_resource_manager: Creating hashmaps\n");
    fflush(stderr);
    material_cache = hashmap_create(hash_string, compare_string);
    fprintf(stderr, "initialize_resource_manager: material_cache=%p\n", (void*)material_cache);
    shader_cache = hashmap_create(hash_string, compare_string);
    fprintf(stderr, "initialize_resource_manager: shader_cache=%p\n", (void*)shader_cache);
    prefab_cache = hashmap_create(hash_string, compare_string);
    fprintf(stderr, "initialize_resource_manager: prefab_cache=%p\n", (void*)prefab_cache);
    fflush(stderr);
}

Material *resource_get_material(const char *material_id)
{
    fprintf(stderr, "resource_get_material: Starting for material_id: %s\n", material_id ? material_id : "NULL");
    fflush(stderr);
    if (!asset_index_json)
    {
        fprintf(stderr, "resource_get_material: Loading asset_index.json\n");
        fflush(stderr);
        asset_index_json = open_json(asset_index_path);
        if (!asset_index_json)
        {
            fprintf(stderr, "resource_get_material: could not parse asset_index.json at path: %s\n", asset_index_path);
            return NULL;
        }
        fprintf(stderr, "resource_get_material: asset_index.json loaded\n");
        fflush(stderr);
    }
    if (!material_cache)
    {
        fprintf(stderr, "resource_get_material: Initializing resource manager\n");
        fflush(stderr);
        initialize_resource_manager();
        fprintf(stderr, "resource_get_material: Resource manager initialized\n");
        fflush(stderr);
    }
    else if (hashmap_contains(material_cache, (void *)material_id))
    {
        fprintf(stderr, "resource_get_material: Found in cache\n");
        return (Material *)hashmap_get(material_cache, (void *)material_id);
    }
    fprintf(stderr, "resource_get_material: Getting path from asset_index\n");
    fflush(stderr);
    char *path = get_path_from_id(material_id, "materials");
    fprintf(stderr, "resource_get_material: Path: %s\n", path ? path : "NULL");
    fflush(stderr);
    cJSON *material_json = open_json(path);
    fprintf(stderr, "resource_get_material: Opened JSON\n");
    fflush(stderr);
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
    fprintf(stderr, "resource_get_material: Parsing name\n");
    fflush(stderr);
    cJSON *name = cJSON_GetObjectItemCaseSensitive(material_json, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        mat->name = strdup(name->valuestring);
    }
    else
    {
        fprintf(stderr, "resource_get_material: material missing name field\n");
        mat->name = strdup("Unnamed Material");
    }
    fprintf(stderr, "resource_get_material: Parsing shader\n");
    fflush(stderr);
    cJSON *shader_id = cJSON_GetObjectItemCaseSensitive(material_json, "shader");
    if (cJSON_IsString(shader_id) && (shader_id->valuestring != NULL))
    {
        fprintf(stderr, "resource_get_material: Loading shader: %s\n", shader_id->valuestring);
        fflush(stderr);
        Shader shader = resource_get_shader(shader_id->valuestring);
        fprintf(stderr, "resource_get_material: Shader loaded\n");
        fflush(stderr);
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

    int i = 0;
    cJSON *gameObject_json = NULL;
    cJSON_ArrayForEach(gameObject_json, gameObjects_json) {
        cJSON *id_json = cJSON_GetObjectItemCaseSensitive(gameObject_json, "id");
        const char *id_const = (id_json && cJSON_IsString(id_json)) ? id_json->valuestring : "GameObject";
        char *id = strdup(id_const);
        GameObject *gameObject = resource_get_prefab(id);
        if (!gameObject) {
            fprintf(stderr, "resource_get_scene: failed to instantiate gameObject with id: %s\n", id);
            free(id);
            continue;
        }
        scene->gameObjects[i] = *gameObject;

        transform_component *transform = get_component(&scene->gameObjects[i], transform_component, "transform_component");
        if (transform) {
            cJSON *position_json = cJSON_GetObjectItemCaseSensitive(gameObject_json, "position");
            if (position_json) {
                printf("parsing vec3 array of floats from gameObject %s", gameObject->name);
                Vector3 position = parse_vector3_array(position_json);
                transform_set_position(transform, position);
                printf("\n\n\n transform.position in resrouce_manager.c: %f, %f, %f \n\n\n", position.x, position.y, position.z);
            }

            cJSON *rotation_json = cJSON_GetObjectItemCaseSensitive(gameObject_json, "rotation");
            if (rotation_json) {
                Vector3 rotation = parse_vector3_array(rotation_json);
                transform_set_rotation(transform, rotation);
            }

            cJSON *scale_json = cJSON_GetObjectItemCaseSensitive(gameObject_json, "scale");
            if (scale_json) {
                Vector3 scale = parse_vector3_array(scale_json);
                transform_set_scale(transform, scale);
            }
        }

        free(id);
        i++;  // CRITICAL: increment i!
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
    fprintf(stderr, "resource_get_mesh: Loading materials for mesh: %s\n", mesh_id);
    cJSON *materials_json = cJSON_GetObjectItemCaseSensitive(mesh_json, "materials");
    Material **materials = malloc(sizeof(Material *) * cJSON_GetArraySize(materials_json));
    int materialCount = 0;
    cJSON_ArrayForEach(iterator, materials_json)
    {
        if (cJSON_IsString(iterator) && (iterator->valuestring != NULL))
        {
            fprintf(stderr, "resource_get_mesh: Loading material: %s\n", iterator->valuestring);
            Material *mat = resource_get_material(iterator->valuestring);
            if (mat)
            {
                fprintf(stderr, "resource_get_mesh: Material loaded successfully\n");
                materials[materialCount] = mat;
                materialCount++;
            }
            else
            {
                fprintf(stderr, "resource_get_mesh: Failed to load material: %s\n", iterator->valuestring);
            }
        }
    }
    fprintf(stderr, "resource_get_mesh: All materials loaded, adding mesh to library\n");

    add_mesh_to_library(strdup(name->valuestring), vertices, indices, cJSON_GetArraySize(indices_json), mesh_id, materials, materialCount);

    // Free temporary arrays after adding to library
    free(vertices);
    free(indices);
    free(materials);

    cJSON_Delete(mesh_json);
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
    fprintf(stderr, "resource_get_prefab: Called with prefab_id: %s\n", prefab_id ? prefab_id : "NULL");
    if (!prefab_id)
    {
        fprintf(stderr, "resource_get_prefab: prefab_id is NULL\n");
        return NULL;
    }
    if (!prefab_cache)
    {
        fprintf(stderr, "resource_get_prefab: Initializing prefab_cache\n");
        initialize_resource_manager();
    }
    fprintf(stderr, "resource_get_prefab: Checking if prefab_id '%s' is in cache (prefab_cache=%p, size=%d)\n",
            prefab_id, (void*)prefab_cache, prefab_cache ? prefab_cache->size : -1);
    fflush(stderr);
    if (prefab_cache && hashmap_contains(prefab_cache, (void *)prefab_id))
    {
        GameObject *prefab = (GameObject *)hashmap_get(prefab_cache, (void *)prefab_id);
        fprintf(stderr, "resource_get_prefab: Retrieved from cache - id: %s, name: %s, components: %d\n",
                prefab_id,
                prefab->name ? prefab->name : "NULL",
                prefab->components_length);
        return prefab;
    }
    resource_load_prefab(prefab_id);
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

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer)
    {
        fprintf(stderr, "open_json: could not allocate memory for file: %s\n", path);
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

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