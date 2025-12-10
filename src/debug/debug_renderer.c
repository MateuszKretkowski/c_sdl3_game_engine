#include "debug_renderer.h"
#include "graphics/render.h"

int DEBUG_OBJECT_MAX_CAPACITY = 128;

debug_object *debug_objects = NULL;
int debug_objects_length = 0;

render_object *debug_get_objects() {
    render_object *r_objects = malloc(sizeof(render_object) * debug_objects_length);
    for (int i=0; i<debug_objects_length; i++) {
        if (debug_objects[i].time_left <= 0) {
            debug_objects[i] = debug_objects[debug_objects_length-1];
            debug_objects_length--;
        }
        debug_objects[i].time_left -= physics_m->timestep;
        r_objects[i] = *debug_objects[i].ro;
    }
    return r_objects;
}

int debug_get_count() {
    return debug_objects_length;
}

void debug_draw_sphere(Vector3 pos, float radius, Vector4 color, float time) {
    if (!debug_objects) {
        debug_objects = malloc(sizeof(debug_object)*128);
    }
    if (radius <= 0) {
        return;
    }
    render_object *ro = malloc(sizeof(render_object));
    Vector3 scale;
    scale.x = 1 * radius;
    scale.y = 1 * radius;
    scale.z = 1 * radius;
    ro->gameObject = NULL;
    ro->transform_component = create_transform_component(pos, vector3_zero(), scale);
    printf("\n\n\n\nBEFORE CREATING MESH_RENDERER\n\n\n\n");
    ro->mesh_renderer_component = create_mesh_renderer_component(resource_get_mesh("sphere"));
    printf("AFTER CREATING MESH_RENDERER\n");
    debug_object *debug_obj = malloc(sizeof(debug_object));
    debug_obj->ro = ro;
    debug_obj->time_left = time;
    debug_objects[debug_objects_length] = *debug_obj;
    debug_objects_length++;
}
