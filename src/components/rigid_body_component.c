#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rigid_body_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "transform_component.h"
#include "utils/resource_manager.h"

#include <cglm/cglm.h>

void rigid_body_awake(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

void rigid_body_start(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

void rigid_body_update(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

void rigid_body_destroy(Component* self) {
    rigid_body_component *comp = (rigid_body_component*)self;
}

rigid_body_component *create_rigid_body_component(physics_material *pmc, bool use_gravity, float mass, Vector3 velocity, bool is_kinematic) {
    rigid_body_component* comp = malloc(sizeof(rigid_body_component));
    comp->base.id = strdup("rigid_body_component");
    comp->base.name = strdup("RigidBody");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(rigid_body_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = rigid_body_awake;
    comp->base.standard_voids->start = rigid_body_start;
    comp->base.standard_voids->update = rigid_body_update;
    comp->base.standard_voids->destroy = rigid_body_destroy;

    comp->physics_material = pmc;
    comp->use_gravity = use_gravity;
    comp->mass = mass;
    comp->velocity = velocity;
    comp->is_kinematic = is_kinematic;
    comp->acceleration = vector3_zero();

    return comp;
}

Component* rigid_body_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: rigid_body_from_json received NULL json\n");
        return NULL;
    }

    cJSON *pmc_id = cJSON_GetObjectItemCaseSensitive(json, "physics_material");
    if (!pmc_id || !cJSON_IsString(pmc_id) || !pmc_id->valuestring) {
        printf("ERROR: rigid_body_from_json received NULL or invalid physics_material\n");
        return NULL;
    }

    physics_material *pmc = resource_get_physics_material(pmc_id->valuestring);
    if (!pmc) {
        printf("ERROR: rigid_body_from_json failed to load physics_material: %s\n", pmc_id->valuestring);
        return NULL;
    }

    cJSON *use_gravity_json = cJSON_GetObjectItemCaseSensitive(json, "use_gravity");
    if (!use_gravity_json) {
        printf("ERROR: rigid_body_from_json failed to load use_gravity.\n");
        return NULL;
    }

    bool use_gravity = cJSON_IsTrue(use_gravity_json);

    cJSON *is_kinematic_json = cJSON_GetObjectItemCaseSensitive(json, "is_kinematic");
    if (!use_gravity_json) {
        printf("ERROR: rigid_body_from_json failed to load is_kinematic.\n");
        return NULL;
    }

    bool is_kinematic = cJSON_IsTrue(is_kinematic_json);

    // Parse mass
    cJSON *mass_json = cJSON_GetObjectItemCaseSensitive(json, "mass");
    float mass = 1.0f; // Default mass
    if (mass_json && cJSON_IsNumber(mass_json)) {
        mass = (float)mass_json->valuedouble;
    }

    // Parse velocity
    cJSON *velocity_json = cJSON_GetObjectItemCaseSensitive(json, "velocity");
    Vector3 velocity = vector3_zero();
    if (velocity_json && cJSON_IsArray(velocity_json)) {
        if (cJSON_GetArraySize(velocity_json) >= 3) {
            velocity.x = (float)cJSON_GetArrayItem(velocity_json, 0)->valuedouble;
            velocity.y = (float)cJSON_GetArrayItem(velocity_json, 1)->valuedouble;
            velocity.z = (float)cJSON_GetArrayItem(velocity_json, 2)->valuedouble;
        }
    }

    return (Component*)create_rigid_body_component(pmc, use_gravity, mass, velocity, is_kinematic);
}

__attribute__((constructor))
static void register_rigid_body_component() {
    component_registry_register("rigid_body_component", rigid_body_from_json);
}
