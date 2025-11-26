#include "spatial_system.h"

spatial_system_config spc;

void spatial_system_init(cJSON *json) {
    cJSON *cell_size = cJSON_GetObjectItemCaseSensitive(json, "cell_size");
    if (!cell_size || !cell_size->valuedouble) {
        printf("spatial_system_init(): cell_size not defined.");
        return;
    }
    spc.cell_size = cell_size->valuedouble;
}