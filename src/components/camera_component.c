#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camera_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"

#include <cglm/cglm.h>

void camera_awake(Component* self) {
    camera_component *cam = (camera_component*)self;
}

void camera_start(Component* self) {
    camera_component *cam = (camera_component*)self;
}

void camera_update(Component* self) {
    camera_component *cam = (camera_component*)self;

    // TODO: Update view and projection matrices here
}

void camera_destroy(Component* self) {
    camera_component *cam = (camera_component*)self;
}

camera_component *create_camera_component() {
    camera_component* cam = malloc(sizeof(camera_component));
    cam->base.id = "camera_component";
    cam->base.name = "Camera";
    cam->base.isActive = true;
    cam->base.gameObject = NULL;
    cam->base.size = sizeof(camera_component);
    cam->base.standard_voids = malloc(sizeof(component_standard_voids));
    cam->base.standard_voids->awake = camera_awake;
    cam->base.standard_voids->start = camera_start;
    cam->base.standard_voids->update = camera_update;
    cam->base.standard_voids->destroy = camera_destroy;

    // TODO: Initialize camera fields here
 
    return cam;
}

Component* camera_from_json(cJSON *json) {
    if (!json) {
        fprintf(stderr, "ERROR: camera_from_json received NULL json\n");
        return NULL;
    }

    // TODO: Parse camera parameters from JSON

    return (Component*)create_camera_component();
}

__attribute__((constructor))
static void register_camera_component() {
    component_registry_register("camera_component", camera_from_json);
}
