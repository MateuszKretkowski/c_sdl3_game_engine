#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <libs/cJSON.h>
#include <utils/utils.h>

Vector2 parse_vector2(cJSON *json);
Vector3 parse_vector3(cJSON *json);
Vector4 parse_vector4(cJSON *json);
Vector2 parse_vector2_array(cJSON *json);
Vector3 parse_vector3_array(cJSON *json);
Vector4 parse_vector4_array(cJSON *json);

#endif // JSON_UTILS_H
