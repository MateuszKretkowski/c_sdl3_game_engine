#include "physics_utils.h"

void add_force(rigid_body_component* rb, Vector3 direction, float force) {
    Vector3 curr_acceleration = vector3_divide(vector3_multiply(direction, force), rb->mass);
    rb->acceleration = curr_acceleration;
}