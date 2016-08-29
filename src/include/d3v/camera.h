#ifndef D3V_CAMERA_H_
#define D3V_CAMERA_H_

#include <stdbool.h>

#include "d3v/math_util.h"
#include "d3v/compiler.h"
#include "d3v/shader.h"

typedef enum d3v_event_type_ {
    UP =   0,
    DOWN = 1,
} d3v_event_type;

typedef struct d3v_camera_ {
    double distance;
    vec4 reye, look;
    vec4 yaxis, xaxis;

    bool updated;
    float view[16];
    float projection[16];
} d3v_camera;

int d3v_camera_init(d3v_camera *camera,
                    vec4 look, float distance, int ay, int ax);

void d3v_camera_free(d3v_camera *c);
void d3v_camera_set_look(d3v_camera *c, vec4 look);
void d3v_camera_set_distance(d3v_camera *c, float distance);

void d3v_camera_set_rotate(d3v_camera *c, float az, float ax);
void d3v_camera_rotate(d3v_camera *c, float ar, float ay);

void d3v_camera_translate(d3v_camera *c, float dx, float dy);
void d3v_camera_add_distance(d3v_camera *c, float d);
void d3v_camera_update(d3v_camera *c, d3v_program *program);

#endif	// D3V_CAMERA_H_
