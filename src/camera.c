#include <stdio.h>
#include <string.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "d3v/camera.h"
#include "d3v/math_util.h"

static void
compute_view(d3v_camera *camera)
{
    vec4 eye = camera->reye;
    SCALAR_MULT4_3(eye, camera->distance);
    ADD4_3(eye, camera->look);
    matrix_look_at(camera->view, eye, camera->look, camera->yaxis);
    camera->updated = true;
}

int d3v_camera_init(d3v_camera *camera, vec4 look, float dis, int ay, int ar)
{
    memset(camera->projection, 0, sizeof camera->projection);
    memset(camera->view, 0, sizeof camera->view);
    matrix_fov_projection(camera->projection, deg2rad(45.), 1., 0.1, 100);

    camera->distance = dis;
    camera->look = look;
    camera->reye = (vec4) { 0., 0., dis, 1.};
    camera->yaxis = (vec4) { 0., 1., 0., 0.};
    camera->xaxis = (vec4) { 1., 0., 0., 0.};

    compute_view(camera);
    return 0;
}

void d3v_camera_free(d3v_camera *camera)
{
    memset(camera, 0, sizeof *camera);
}

void d3v_camera_set_look(d3v_camera *camera, vec4 look)
{
    camera->look = look;
    compute_view(camera);
}

void d3v_camera_set_distance(d3v_camera *camera, float d)
{
    camera->distance = d;
    compute_view(camera);
}

void d3v_camera_translate(d3v_camera *camera, float dw, float dh)
{
    vec4 tmp = {
	dw * camera->xaxis.x + dh * camera->yaxis.x,
	dw * camera->xaxis.y + dh * camera->yaxis.y,
	dw * camera->xaxis.z + dh * camera->yaxis.z,
        0.
    };

    ADD4(camera->look, tmp);
    compute_view(camera);
}

void d3v_camera_add_distance(d3v_camera *camera, float d)
{
    camera->distance += d;
    if (camera->distance < 0)
        camera->distance = 0;
    compute_view(camera);
}

void d3v_camera_rotate(d3v_camera *camera, float ax, float ay)
{
    float R[16];
    matrix_rotation(R, ax, camera->xaxis);
    matrix_vector_multiply(R, &camera->reye);
    matrix_vector_multiply(R, &camera->yaxis);
    
    vec4 up = { 0., 1., 0., 0. };
    matrix_rotation(R, ay, up);
    matrix_vector_multiply(R, &camera->reye);
    matrix_vector_multiply(R, &camera->xaxis);
    matrix_vector_multiply(R, &camera->yaxis);
    compute_view(camera);
}

void d3v_camera_update(d3v_camera *camera, d3v_program *program)
{
    if (camera->updated) {
        int v = program->view_location;
        int p = program->projection_location;
        glUniformMatrix4fv(v, 1, GL_FALSE, &(camera->view[0]));
        glUniformMatrix4fv(p, 1, GL_FALSE, &(camera->projection[0]));
        camera->updated = false;
    }
}
