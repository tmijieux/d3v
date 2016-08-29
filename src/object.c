#include <stdio.h>
#include <glib.h>

#include "d3v/object.h"
#include "d3v/model.h"
#include "d3v/math_util.h"
#include "d3v/texture.h"


static void compute_model(d3v_object *obj)
{
    float T[16];
    matrix_multiply(obj->translate, obj->rotate, T);
    matrix_multiply(T, obj->scale, obj->model_matrix);
}

int
d3v_object_init(d3v_object *obj,
                d3v_model *model, d3v_texture* texture,
                vec3 pos, float yrot, float scale)
{
    vec4 up = {0., 1., 0., 0.};
    matrix_translation(obj->translate, pos);
    matrix_rotation(obj->rotate, deg2rad(yrot), up);
    matrix_scale(obj->scale, (vec3){scale, scale, scale});
    
    compute_model(obj);
    obj->model = model;

    if (texture != NULL) {
	obj->tex_id = d3v_texture_get_id(texture);
	obj->textured = true;
    }
    return 0;
}

d3v_object*
d3v_object_create(d3v_model *model, d3v_texture* texture,
                  vec3 pos, float yrot, float scale)
{
    d3v_object *obj = g_malloc0(sizeof(*obj));
    d3v_object_init(obj, model, texture, pos, yrot, scale);
    return obj;
}

void d3v_object_set_shademodel(d3v_object *obj, int shademodel)
{
    obj->shademodel = shademodel;
}

void d3v_object_set_position(d3v_object *obj, vec3 pos)
{
    matrix_translation(obj->translate, pos);
    compute_model(obj);
}

void d3v_object_translate(d3v_object *obj, vec3 pos)
{
    //matrix_translate(obj->model_matrix, &pos);
}

void d3v_object_draw(d3v_object *obj)
{
    if (obj == NULL || obj->hidden)
	return;

    int m = obj->program->model_location;
    glUniformMatrix4fv(m, 1, GL_FALSE, &(obj->model_matrix[0]));
    d3v_model_draw(obj->model, obj->program);
}

void d3v_object_set_program(d3v_object *obj, d3v_program *prog)
{
    obj->program = prog;
}

void d3v_object_hide(d3v_object *obj)
{
    obj->hidden = true;
}

void d3v_object_reveal(d3v_object *obj)
{
    obj->hidden = false;
}

void d3v_object_free(d3v_object *obj)
{
    g_free(obj);
}
