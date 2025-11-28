#include "spatial_system.h"

spatial_system_config spc;
grid_cell *spatial;
int spacial_length;

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
    for (int i = 0; i < spc.grid_width * spc.grid_height * spc.grid_depth; i++) {
        // for now:
        spatial[i].count = 0;
        // someday: calculating which go's didn't move in the frame, and not calculating them.
    }
    for (int i=0; i<spacial_length; i++) {
        transform_component *transform = get_component(&objects[i], transform_component, "transform_component");
        int x = (int)round(transform->position.x / spc.cell_size);
        int y = (int)round(transform->position.y / spc.cell_size);
        int z = (int)round(transform->position.z / spc.cell_size);
        if (x>=spc.grid_width || x<0 || y>=spc.grid_height || y<0 || z>=spc.grid_depth || z<0) {
            printf("spatial_system_realloc_objects(): object to spatial is out of bounds.\n");
            continue;
        }
        int index = x * (spc.grid_height * spc.grid_depth) + y * spc.grid_depth + z;
        grid_cell *gc = &spatial[index];
        if (!gc->objects) {
            gc->capacity = spc.grid_cell_capacity;
            gc->objects = malloc(sizeof(GameObject*)*gc->capacity);
            gc->count = 0;
        }
        else if (gc->count >= gc->capacity) {
            gc->capacity *= 2;
            gc->objects = realloc(gc->objects, sizeof(GameObject*)*gc->capacity);
        }
        gc->objects[gc->count] = objects[i];
        gc->count++;
    }
}

void spatial_system_free() {
    for (int i=0; i<spc.grid_width*spc.grid_height*spc.grid_depth; i++) {
        free(spatial[i].objects);
    }
    free(spatial);
}

void spatial_system_update(GameObject *gameObjects) {
    spatial_system_realloc_objects(gameObjects);
}