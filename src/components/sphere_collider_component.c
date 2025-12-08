#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#include "sphere_collider_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"

#include <cglm/cglm.h>

void sphere_collider_awake(Component* self) {
    sphere_collider_component *comp = (sphere_collider_component*)self;
}

void sphere_collider_start(Component* self) {
    sphere_collider_component *comp = (sphere_collider_component*)self;
}

void sphere_collider_update(Component* self) {
    sphere_collider_component *comp = (sphere_collider_component*)self;
}

void sphere_collider_destroy(Component* self) {
    sphere_collider_component *comp = (sphere_collider_component*)self;
}

sphere_collider_component *create_sphere_collider_component(Vector3 pos, float radius) {
    sphere_collider_component* comp = malloc(sizeof(sphere_collider_component));
    if (!comp) return NULL; 
    
    comp->base.id = strdup("sphere_collider_component");
    comp->base.name = strdup("sphere_collider");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(sphere_collider_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    
    if (!comp->base.standard_voids) {
        free(comp->base.id);
        free(comp->base.name);
        free(comp);
        return NULL;
    }
    
    comp->base.standard_voids->awake = sphere_collider_awake;
    comp->base.standard_voids->start = sphere_collider_start;
    comp->base.standard_voids->update = sphere_collider_update;
    comp->base.standard_voids->destroy = sphere_collider_destroy;

    comp->pos = pos;
    comp->radius = radius;

    return comp;
}

Component* sphere_collider_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: sphere_collider_from_json received NULL json\n");
        return NULL;
    }
    cJSON *pos_json = cJSON_GetObjectItemCaseSensitive(json, "position");
    if (!pos_json) {
        printf("Could not get position from sphere_collider_component json.\n");
        return NULL;
    }
    Vector3 pos = parse_vector3_array(pos_json);
    cJSON *radius = cJSON_GetObjectItemCaseSensitive(json, "radius");
    if (!radius || !radius->valuedouble) {
        printf("Could not get radius from sphere_collider_component json.\n");
        return NULL;
    }
    return (Component*)create_sphere_collider_component(pos, (float)radius->valuedouble);
}

__attribute__((constructor))
static void register_sphere_collider_component() {
    component_registry_register("sphere_collider_component", sphere_collider_from_json);
}