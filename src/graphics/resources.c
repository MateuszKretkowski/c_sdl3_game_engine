#include <stdio.h>
#include <stdlib.h>
#include "resources.h"
#include "textures.h"

GLuint tex_bricks = 0;
GLuint tex_wood = 0;

bool resources_load(void) {
    tex_bricks = generate_texture("brick_texture/bricks_color.png");
    tex_wood   = generate_texture("wood_texture/wood_color.png");

    if (!tex_bricks || !tex_wood) {
        fprintf(stderr, "FAILED TO LOAD TEX_BRICKS OR TEX_WOOD: RESOURCES.C");
        return false;
    }

    return true;
}

void resources_unload(void) {
    if (tex_bricks) glDeleteTextures(1, &tex_bricks);
    if (tex_wood)   glDeleteTextures(1, &tex_wood);
}
