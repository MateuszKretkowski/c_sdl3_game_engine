#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include "utils.h"
#include "shader.h"

void material_bind(Material *mat) {
    shader_use(mat->shader);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse_map.id);
    shader_set_int(mat->shader, "diffuse_map", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat->specular_map.id);
    shader_set_int(mat->shader, "specular_map", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mat->normal_map.id);
    shader_set_int(mat->shader, "normal_map", 2);

    shader_set_float(mat->shader, "shininess", mat->shininess);
}