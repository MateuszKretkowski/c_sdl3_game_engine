#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soft_body_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"
#include "utils/resource_manager.h"

#include <cglm/cglm.h>

void soft_body_awake(Component* self) {
    soft_body_component *comp = (soft_body_component*)self;
}

void soft_body_start(Component* self) {
    soft_body_component *comp = (soft_body_component*)self;

}

void soft_body_update(Component* self) {
    soft_body_component *comp = (soft_body_component*)self;
}

void soft_body_destroy(Component* self) {
    soft_body_component *comp = (soft_body_component*)self;
    
    // Free gameObjects array
    if (comp->gameObjects) {
        free(comp->gameObjects);
        comp->gameObjects = NULL;
    }
    
    // NOTE: Do NOT free standard_voids, id, name here!
    // They are freed by local_destroy_component in component.c
}

soft_body_component *create_soft_body_component(physics_material *pm, float mass, bool use_gravity, bool is_kinematic, int child_count) {
    if (!pm) {
        fprintf(stderr, "ERROR: create_soft_body_component received NULL physics_material\n");
        return NULL;
    }
    
    soft_body_component* comp = malloc(sizeof(soft_body_component));
    if (!comp) {
        fprintf(stderr, "ERROR: Failed to allocate soft_body_component\n");
        return NULL;
    }
    
    comp->base.id = strdup("soft_body_component");
    comp->base.name = strdup("SoftBody");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(soft_body_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    if (!comp->base.standard_voids) {
        fprintf(stderr, "ERROR: Failed to allocate component_standard_voids\n");
        free(comp);
        return NULL;
    }
    comp->base.standard_voids->awake = soft_body_awake;
    comp->base.standard_voids->start = soft_body_start;
    comp->base.standard_voids->update = soft_body_update;
    comp->base.standard_voids->destroy = soft_body_destroy;

    comp->physics_material = pm;
    comp->mass = mass;
    comp->use_gravity = use_gravity;
    comp->is_kinematic = is_kinematic;
    comp->child_count = child_count;

    // Initialize velocity to zero
    comp->velocity = vector3_zero();

    // Allocate memory for gameObjects array only if child_count > 0
    if (child_count > 0) {
        comp->gameObjects = malloc(sizeof(GameObject*) * child_count);
        if (comp->gameObjects) {
            for (int i = 0; i < child_count; i++) {
                comp->gameObjects[i] = NULL;
            }
        }
    } else {
        comp->gameObjects = NULL;
    }

    return comp;
}

Component* soft_body_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: soft_body_from_json received NULL json\n");
        return NULL;
    }

    physics_material *pm = NULL;
    cJSON *pm_id = cJSON_GetObjectItemCaseSensitive(json, "physics_material");
    if (pm_id && cJSON_IsString(pm_id) && pm_id->valuestring) {
        pm = resource_get_physics_material(pm_id->valuestring);
        if (!pm) {
            fprintf(stderr, "ERROR: soft_body_from_json failed to load physics_material: %s\n", pm_id->valuestring);
            return NULL;
        }
    } else {
        fprintf(stderr, "ERROR: soft_body_from_json missing physics_material in JSON\n");
        return NULL;
    }

    float mass = 1.0f;
    cJSON *mass_json = cJSON_GetObjectItemCaseSensitive(json, "mass");
    if (mass_json && cJSON_IsNumber(mass_json)) {
        mass = (float)mass_json->valuedouble;
    }

    bool use_gravity = true;
    cJSON *use_gravity_json = cJSON_GetObjectItemCaseSensitive(json, "use_gravity");
    if (use_gravity_json && cJSON_IsBool(use_gravity_json)) {
        use_gravity = cJSON_IsTrue(use_gravity_json);
    }

    bool is_kinematic = false;
    cJSON *is_kinematic_json = cJSON_GetObjectItemCaseSensitive(json, "is_kinematic");
    if (is_kinematic_json && cJSON_IsBool(is_kinematic_json)) {
        is_kinematic = cJSON_IsTrue(is_kinematic_json);
    }

    int child_count = 0;
    cJSON *child_count_json = cJSON_GetObjectItemCaseSensitive(json, "child_count");
    if (child_count_json && cJSON_IsNumber(child_count_json)) {
        child_count = (int)child_count_json->valueint;
    }

    return (Component*)create_soft_body_component(pm, mass, use_gravity, is_kinematic, child_count);
}

__attribute__((constructor))
static void register_soft_body_component() {
    component_registry_register("soft_body_component", soft_body_from_json);
}
