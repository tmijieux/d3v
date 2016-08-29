#ifndef D3V_OBJECT_H
#define D3V_OBJECT_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>


#include "d3v/texture.h"
#include "d3v/model.h"
#include "d3v/shader.h"

typedef struct d3v_object_ {
    float translate[16];
    float rotate[16];
    float scale[16];
    float model_matrix[16];
    
    d3v_model *model;
    GLuint tex_id;
    GLint shademodel;

    bool textured;
    bool colored;
    bool hidden;

    d3v_program *program;
} d3v_object;

d3v_object* d3v_object_create(d3v_model *m, d3v_texture* t,
                              vec3 pos, float yrot, float scale);
void d3v_object_free(d3v_object *obj);
void d3v_object_draw(d3v_object *obj);
void d3v_object_hide(d3v_object *obj);
void d3v_object_reveal(d3v_object *obj);
void d3v_object_set_shademodel(d3v_object*, int shademodel);
void d3v_object_set_program(d3v_object *obj, d3v_program *prog);

#endif // D3V_OBJECT_H
