#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <string.h>
#include <dirent.h>
#include <cJSON.h>

const char *asset_index_path = "src/assets/asset_index.json";

const char *extension = ".json";
const char *materials_path = "src/assets/materials/";
const char *meshes_path = "src/assets/meshes/";
const char *prefabs_path = "src/assets/prefabs/";
const char *shaders_path = "src/assets/shaders/";

void id_json(cJSON *asset_index_json, const char *path, const char *category) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("ERROR: Cannot open '%s'\n", path);
        printf("Current directory: %s\n", cwd);
        return;
    }
    cJSON *category_obj = cJSON_GetObjectItemCaseSensitive(asset_index_json, category);
    if (!category_obj) {
        printf("ERROR: Category '%s' not found\n", category);
        closedir(dir);
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strlen(entry->d_name) > strlen(extension) && 
            strcmp(entry->d_name + strlen(entry->d_name) - strlen(extension), extension) == 0) {
                printf("found JSON file: %s\n", entry->d_name);
                // otwieranie filesa:
                char filePath[2048];
                snprintf(filePath, sizeof(filePath), "%s%s", path, entry->d_name);
                FILE *currFile = fopen(filePath, "r");
                if (!currFile) {
                    printf("ERROR: Cannot open file %s\n", filePath);
                    continue;
                }
                printf("filePath: %s\n", filePath);
                fseek(currFile, 0, SEEK_END);
                long fileSize = ftell(currFile);
                fseek(currFile, 0, SEEK_SET);
                char *buffer = malloc(fileSize + 1);
                fread(buffer, 1, fileSize, currFile);
                buffer[fileSize] = '\0';
                fclose(currFile);
                // cJSON:
                cJSON *json = cJSON_Parse(buffer);
                free(buffer);
                if (json == NULL) {
                    const char *error = cJSON_GetErrorPtr();
                    if (error != NULL) {
                        printf("Error: %s\n", error);
                    }
                    cJSON_Delete(json);
                    continue;
                }
                cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "id");
                if (cJSON_IsString(id) && (id->valuestring != NULL)) {
                    printf("id: %s\n", id->valuestring);
                    cJSON_AddStringToObject(category_obj, id->valuestring, filePath);
                }
                else {
                    printf("ERROR: CANNOT GET ID from %s\n", filePath);
                }
                cJSON_Delete(json);
            }
    }
    closedir(dir);
} 

cJSON *get_asset_index_json() {
    FILE *asset_index_file = fopen(asset_index_path, "r");
    if (!asset_index_file) {
        return NULL;
    }
    fseek(asset_index_file, 0, SEEK_END);
    long fileSize = ftell(asset_index_file);
    fseek(asset_index_file, 0, SEEK_SET);
    
    char *buffer = malloc(fileSize + 1);
    fread(buffer, 1, fileSize, asset_index_file);
    buffer[fileSize] = '\0';
    fclose(asset_index_file);
    
    cJSON *asset_index_json = cJSON_Parse(buffer);
    free(buffer);
    if (asset_index_json == NULL) {
        const char *error = cJSON_GetErrorPtr();
        if (error != NULL) {
            printf("Error parsing asset_index: %s\n", error);
        }
        return NULL;
    }
    return asset_index_json;
}

cJSON *set_up_asset_index_json() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddObjectToObject(json, "materials");
    cJSON_AddObjectToObject(json, "meshes");
    cJSON_AddObjectToObject(json, "prefabs");
    cJSON_AddObjectToObject(json, "shaders");
    return json;
}

void upload_asset_index_json(cJSON *asset_index_json) {
    FILE *file = fopen(asset_index_path, "w");
    if (!file) {
        printf("ERROR: Cannot open file for writing: %s\n", asset_index_path);
        return;
    }
    
    char *json_string = cJSON_Print(asset_index_json);
    if (json_string) {
        fprintf(file, "%s", json_string);
        free(json_string);
        printf("Asset index saved to: %s\n", asset_index_path);
    } else {
        printf("ERROR: Cannot serialize JSON\n");
    }
    
    fclose(file);
}

int main() {
    cJSON *asset_index_json = set_up_asset_index_json();
    
    id_json(asset_index_json, materials_path, "materials");
    id_json(asset_index_json, meshes_path, "meshes");
    id_json(asset_index_json, prefabs_path, "prefabs");
    id_json(asset_index_json, shaders_path, "shaders");
    
    upload_asset_index_json(asset_index_json);
    
    cJSON_Delete(asset_index_json);
    return 0;
}