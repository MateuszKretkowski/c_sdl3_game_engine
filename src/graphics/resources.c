#include <stdio.h>
#include <stdlib.h>
#include "resources.h"
#include "textures.h"

GLuint tex_bricks = 0;
GLuint tex_wood = 0;

bool resources_load(void) {
    tex_bricks = create_texture("brick_texture/bricks_color.png");
    tex_wood   = create_texture("wood_texture/wood_color.png");

    if (!tex_bricks || !tex_wood) {
        fprintf(stderr, "resources_load: failed to load textures - tex_bricks: %u, tex_wood: %u\n", tex_bricks, tex_wood);
        return false;
    }

    return true;
}

void resources_unload(void) {
    if (tex_bricks) glDeleteTextures(1, &tex_bricks);
    if (tex_wood)   glDeleteTextures(1, &tex_wood);
}
