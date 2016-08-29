#ifndef D3V_LIGHT_H
#define D3V_LIGHT_H

#include "math_util.h"

typedef struct d3v_light_ {
    vec4 pos;
    vec4 dif;
    vec4 dir;
    vec4 Mspec;
    GLfloat Mshiny;
    
} d3v_light;

int d3v_light_init(d3v_light *light);
void d3v_light_update(d3v_light *light);
void d3v_light_free(d3v_light *light);

#endif // D3V_LIGHT_H
