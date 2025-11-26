#include "spatial_system.h"

spatial_system_config spc;
GameObject *spatial;

void spatial_system_realloc_objects(GameObject *objects);

void spatial_system_init(GameObject *objects, int gameObject_length, cJSON *json) {
    cJSON *cell_size = cJSON_GetObjectItemCaseSensitive(json, "cell_size");
    if (!cell_size || !cell_size->valuedouble) {
        printf("spatial_system_init(): cell_size not defined.");
        return;
    }
    spc.cell_size = cell_size->valuedouble;
    cJSON *grid_width = cJSON_GetObjectItemCaseSensitive(json, "grid_width");
    if (!grid_width || !grid_width->valueint) {
        printf("spatial_system_init(): grid_width not defined.");
        return;
    }
    spc.grid_width = grid_width->valueint;
    cJSON *grid_height = cJSON_GetObjectItemCaseSensitive(json, "grid_height");
    if (!grid_height || !grid_height->valueint) {
        printf("spatial_system_init(): grid_height not defined.");
        return;
    }
    spc.grid_height = grid_height->valueint;

    spatial = malloc(spc.grid_width*spc.grid_height*spc.grid_depth*sizeof(grid_cell));
    spatial_system_realloc_objects(objects);
}

void spatial_system_realloc_objects(GameObject *objects) {

}

void spatial_system_update(GameObject *gameObjects) {

}