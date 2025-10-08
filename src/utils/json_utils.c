#include "json_utils.h"
#include <stdio.h>

Vector2 parse_vector2(cJSON *json) {
    Vector2 v = {0};

    if (!json) {
        fprintf(stderr, "ERROR: parse_vector2 received NULL json\n");
        return v;
    }

    cJSON *x = cJSON_GetObjectItemCaseSensitive(json, "x");
    cJSON *y = cJSON_GetObjectItemCaseSensitive(json, "y");

    if (cJSON_IsNumber(x)) {
        v.x = (float)x->valuedouble;
    }
    if (cJSON_IsNumber(y)) {
        v.y = (float)y->valuedouble;
    }

    return v;
}

Vector3 parse_vector3(cJSON *json) {
    Vector3 v = {0};

    if (!json) {
        fprintf(stderr, "ERROR: parse_vector3 received NULL json\n");
        return v;
    }

    cJSON *x = cJSON_GetObjectItemCaseSensitive(json, "x");
    cJSON *y = cJSON_GetObjectItemCaseSensitive(json, "y");
    cJSON *z = cJSON_GetObjectItemCaseSensitive(json, "z");

    if (cJSON_IsNumber(x)) {
        v.x = (float)x->valuedouble;
    }
    if (cJSON_IsNumber(y)) {
        v.y = (float)y->valuedouble;
    }
    if (cJSON_IsNumber(z)) {
        v.z = (float)z->valuedouble;
    }

    return v;
}

Vector4 parse_vector4(cJSON *json) {
    Vector4 v = {0};

    if (!json) {
        fprintf(stderr, "ERROR: parse_vector4 received NULL json\n");
        return v;
    }

    cJSON *x = cJSON_GetObjectItemCaseSensitive(json, "x");
    cJSON *y = cJSON_GetObjectItemCaseSensitive(json, "y");
    cJSON *z = cJSON_GetObjectItemCaseSensitive(json, "z");
    cJSON *w = cJSON_GetObjectItemCaseSensitive(json, "w");

    if (cJSON_IsNumber(x)) {
        v.x = (float)x->valuedouble;
    }
    if (cJSON_IsNumber(y)) {
        v.y = (float)y->valuedouble;
    }
    if (cJSON_IsNumber(z)) {
        v.z = (float)z->valuedouble;
    }
    if (cJSON_IsNumber(w)) {
        v.w = (float)w->valuedouble;
    }

    return v;
}

Vector2 parse_vector2_array(cJSON *json) {
    Vector2 v = {0};

    if (!json || !cJSON_IsArray(json)) {
        fprintf(stderr, "ERROR: parse_vector2_array received NULL or non-array json\n");
        return v;
    }

    if (cJSON_GetArraySize(json) >= 2) {
        cJSON *item0 = cJSON_GetArrayItem(json, 0);
        cJSON *item1 = cJSON_GetArrayItem(json, 1);

        if (cJSON_IsNumber(item0)) {
            v.x = (float)item0->valuedouble;
        }
        if (cJSON_IsNumber(item1)) {
            v.y = (float)item1->valuedouble;
        }
    }

    return v;
}

Vector3 parse_vector3_array(cJSON *json) {
    Vector3 v = {0};

    if (!json || !cJSON_IsArray(json)) {
        fprintf(stderr, "ERROR: parse_vector3_array received NULL or non-array json\n");
        return v;
    }

    if (cJSON_GetArraySize(json) >= 3) {
        cJSON *item0 = cJSON_GetArrayItem(json, 0);
        cJSON *item1 = cJSON_GetArrayItem(json, 1);
        cJSON *item2 = cJSON_GetArrayItem(json, 2);

        if (cJSON_IsNumber(item0)) {
            v.x = (float)item0->valuedouble;
        }
        if (cJSON_IsNumber(item1)) {
            v.y = (float)item1->valuedouble;
        }
        if (cJSON_IsNumber(item2)) {
            v.z = (float)item2->valuedouble;
        }
    }

    return v;
}

Vector4 parse_vector4_array(cJSON *json) {
    Vector4 v = {0};

    if (!json || !cJSON_IsArray(json)) {
        fprintf(stderr, "ERROR: parse_vector4_array received NULL or non-array json\n");
        return v;
    }

    if (cJSON_GetArraySize(json) >= 4) {
        cJSON *item0 = cJSON_GetArrayItem(json, 0);
        cJSON *item1 = cJSON_GetArrayItem(json, 1);
        cJSON *item2 = cJSON_GetArrayItem(json, 2);
        cJSON *item3 = cJSON_GetArrayItem(json, 3);

        if (cJSON_IsNumber(item0)) {
            v.x = (float)item0->valuedouble;
        }
        if (cJSON_IsNumber(item1)) {
            v.y = (float)item1->valuedouble;
        }
        if (cJSON_IsNumber(item2)) {
            v.z = (float)item2->valuedouble;
        }
        if (cJSON_IsNumber(item3)) {
            v.w = (float)item3->valuedouble;
        }
    }

    return v;
}
