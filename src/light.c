#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "d3v/light.h"

int d3v_light_init(d3v_light *light)
{
    memset(light, 0, sizeof*light);
    
    light->pos =   (vec4) { 2.0,  2.0,  2.0, 0. };
    light->dif =   (vec4) { 1.0,  1.0,  1.0, 0.0 };
    light->dir =   (vec4) { -1.0, -1.0, -1.0, 0.0 };
    light->Mspec = (vec4) { 1.0,  1.0,  1.0, 0.0 };
    light->Mshiny = 80.;

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    return 0;
}

void d3v_light_update(d3v_light *light)
{
    vec4 *light_pos = &light->pos;
    const float pos[4] = {
        light_pos->x,
        light_pos->y,
        light_pos->z,
        light_pos->w,
    };

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void d3v_light_free(d3v_light *light)
{
    (void) light;
}
