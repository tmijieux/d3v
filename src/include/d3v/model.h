#ifndef D3V_MODEL_H
#define D3V_MODEL_H

#include <stdbool.h>
#include "d3v/math_util.h"
#include "d3v/shader.h"

typedef struct d3v_vertex_ {
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
    GLfloat s, t;
} d3v_vertex;

typedef struct d3v_model_ {
    char *file_path;
    char *model_name;

    bool have_texture, have_normal;
    d3v_vertex *vertices;
    unsigned vertices_count;
    GLuint vao, vbo;
} d3v_model;


d3v_model *
d3v_model_new_from_wavefront(char const *file_path, char const *model_name,
                             d3v_program *program);
d3v_model *
d3v_get_model_by_name(char const *model_name);

int d3v_model_init(d3v_model*, char const *file_path,char const *model_name,
                   d3v_program *program);
void d3v_model_draw(d3v_model*, d3v_program *prog);
void d3v_model_dump(d3v_model*);
void d3v_model_free(d3v_model*);
void d3v_model_delete(d3v_model *model);

#endif // D3V_MODELS_H
