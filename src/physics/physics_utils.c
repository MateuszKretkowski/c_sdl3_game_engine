#include "physics_utils.h"

void add_force(rigid_body_component* rb, Vector3 direction, float force) {
    if (!rb) {
        printf("ERROR: add_force(): no rigid_body_component found.\n");
        return;
    }
    Vector3 curr_acceleration = vector3_divide(vector3_multiply(direction, force), rb->mass);
    rb->acceleration = vector3_add(rb->acceleration, curr_acceleration);
}