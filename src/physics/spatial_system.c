#include "spatial_system.h"

spatial_system_config spc;
grid_cell *spatial = NULL;

void spatial_system_realloc_objects(GameObject **objects, int objects_length);

void spatial_system_init(GameObject **objects, int objects_length) {
    cJSON *json = scene_get_scene_config();
    if (!json) {
        printf("spatial_system_init(): ERROR: COULD NOT GET SCENE_CONFIG.");
        return;
    }
    cJSON *spatial_config = cJSON_GetObjectItemCaseSensitive(json, "spatial");
    if (!spatial_config) {
        printf("spatial_system_init(): ERROR: SPATIAL CONFIG NOT FOUND.\n");
        return;
    }
    
    cJSON *cell_size = cJSON_GetObjectItemCaseSensitive(spatial_config, "cell_size");
    if (!cell_size || !cell_size->valuedouble) {
        printf("spatial_system_init(): ERROR: CELL_SIZE NOT DEFINED.\n");
        return;
    }
    spc.cell_size = cell_size->valuedouble;
    
    cJSON *grid_width = cJSON_GetObjectItemCaseSensitive(spatial_config, "grid_width");
    if (!grid_width || !grid_width->valueint) {
        printf("spatial_system_init(): ERROR: GRID_WIDTH NOT DEFINED.\n");
        return;
    }
    spc.grid_width = grid_width->valueint;
    
    cJSON *grid_height = cJSON_GetObjectItemCaseSensitive(spatial_config, "grid_height");
    if (!grid_height || !grid_height->valueint) {
        printf("spatial_system_init(): ERROR: GRID_HEIGHT NOT DEFINED.\n");
        return;
    }
    spc.grid_height = grid_height->valueint;
    
    cJSON *grid_depth = cJSON_GetObjectItemCaseSensitive(spatial_config, "grid_depth");
    if (!grid_depth || !grid_depth->valueint) {
        printf("spatial_system_init(): ERROR: GRID_DEPTH NOT DEFINED.\n");
        return;
    }
    spc.grid_depth = grid_depth->valueint;
    
    cJSON *grid_cell_capacity = cJSON_GetObjectItemCaseSensitive(spatial_config, "grid_cell_capacity");
    if (!grid_cell_capacity || !grid_cell_capacity->valueint) {
        printf("spatial_system_init(): grid_cell_capacity not defined.\n");
        spc.grid_cell_capacity = 1024;
    } else {
        spc.grid_cell_capacity = grid_cell_capacity->valueint;
    }

    spatial = malloc(spc.grid_width*spc.grid_height*spc.grid_depth*sizeof(grid_cell));
    spatial_system_realloc_objects(objects, objects_length);
}

void spatial_system_realloc_objects(GameObject **objects, int objects_length) {
    for (int i = 0; i < spc.grid_width * spc.grid_height * spc.grid_depth; i++) {
        spatial[i].count = 0;
    }
    for (int i=0; i<objects_length; i++) {
        GameObject *curr = objects[i];
        transform_component *transform = get_component(objects[i], transform_component, "transform_component");
        if (!transform) continue;
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

void spatial_system_check_collisions(grid_cell *gc) {
    if (!gc || !gc->objects) {
        return;
    }
    if (gc->count <= 1) {
        return;
    }
    for (int i=0; i<gc->count; i++) {
        GameObject *curr = gc->objects[i];

        sphere_collider_component *curr_sphere = get_component(curr, sphere_collider_component, "sphere_collider_component");
        box_collider_component *curr_box = get_component(curr, box_collider_component, "box_collider_component");

        for (int j=i+1; j<gc->count; j++) {
            GameObject *col_curr = gc->objects[j];
            if (curr == col_curr) {
                continue;
            }
            
            sphere_collider_component *collision_sphere = get_component(col_curr, sphere_collider_component, "sphere_collider_component");
            box_collider_component *collision_box = get_component(col_curr, box_collider_component, "box_collider_component");
            if (curr_sphere && collision_sphere) {
                Vector3 normal = intersect_sphere_sphere(curr_sphere, collision_sphere);
                if (!vector3_compare(normal, vector3_zero())) {
                    printf("COLLISION! SPHERE_SPHERE: %s, %s\n", curr->id, col_curr->id);
                    physics_manager_handle_collision(curr, col_curr, normal);
                }
            }
            else if (curr_sphere && collision_box) {
                Vector3 normal = intersect_AABB_sphere(collision_box, curr_sphere);
                if (!vector3_compare(normal, vector3_zero())) {
                    printf("COLLISION! SPHERE BOX\n");
                    physics_manager_handle_collision(curr, col_curr, normal);
                }
            }
            else if (curr_box && collision_sphere) {
                Vector3 normal = intersect_AABB_sphere(curr_box, collision_sphere);
                if (!vector3_compare(normal, vector3_zero())) {
                    printf("COLLISION! BOX SPHERE\n");
                    physics_manager_handle_collision(curr, col_curr, normal);
                }
            }
            else if (curr_box && collision_box) {
                Vector3 normal = intersect_sphere_sphere(curr_sphere, collision_sphere);
                if (!vector3_compare(normal, vector3_zero())) {
                    printf("COLLISION! BOX BOX\n");
                    physics_manager_handle_collision(curr, col_curr, normal);
                }
            }
        }
    }
}

void spatial_system_free() {
    for (int i=0; i<spc.grid_width*spc.grid_height*spc.grid_depth; i++) {
        free(spatial[i].objects);
    }
    free(spatial);
}

void spatial_system_update(GameObject **gameObjects, int objects_length) {
    spatial_system_realloc_objects(gameObjects, objects_length);
    for (int i=0; i<spc.grid_width * spc.grid_height * spc.grid_depth; i++) {
        spatial_system_check_collisions(&spatial[i]);
    }
}