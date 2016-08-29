#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v/math_util.h"
#include "d3v/model.h"
#include "d3v/util.h"
#include "d3v/error.h"

#define BUFSIZE 1024

typedef struct d3v_model_scaninfo_ {
    bool have_texture;
    bool have_normal;
    
    unsigned vertex_count;
    unsigned normal_count;
    unsigned tex_coord_count;
    unsigned face_count;
} d3v_model_scaninfo;

#define OBJECT_TYPES(TYPE)                      \
    TYPE(OBJECT, "o ")                          \
    TYPE(VERTEX, "v ")                          \
    TYPE(TEX_COORD, "vt")                       \
    TYPE(NORMAL, "vn")                          \
    TYPE(FACE, "f ")                            \

#define TYPE_TO_ENUM(caps_, type_str_)          \
    OBJECT_##caps_,

enum object_type {
    OBJECT_TYPES(TYPE_TO_ENUM)
    MAX_OBJECT_TYPE
};


#define MATCH_OBJECT_TYPE(caps_, type_str_)     \
    do {                                        \
        if (!strcmp(type_str, type_str_))       \
            return OBJECT_##caps_;              \
    } while (0);

static int
get_object_type(char *type_str)
{
    OBJECT_TYPES(MATCH_OBJECT_TYPE);
    return -1;
}

// returns the count of vertex in the face described in line

static unsigned
face_vertex_count(char const *line)
{
    char *face, *tok, *saveptr = NULL;
    unsigned count = 0;

    face = g_strdup(line);
    tok = strtok_r(face, " ", &saveptr);
    while (tok != NULL) {
        ++ count;
        tok = strtok_r(NULL, " ", &saveptr);
    }

    g_free(face);
    return count;
}

static int
scan_wavefront_file(GIOChannel *chan, d3v_model_scaninfo *info)
     
{
    int err = 0;
    char *line = NULL;
    memset(info, 0, sizeof*info);

    for (;!(err = d3v_readline(chan, &line)); g_free(line)) {
        if (line[0] == 'v') {
            switch (line[1]) {
            case ' ': ++ info->vertex_count; break;
            case 't': ++ info->tex_coord_count; break;
            case 'n': ++ info->normal_count; break;
            default: return -1; break;
            }
        } else if (line[0] == 'f' && line[1] == ' ') {
            if (face_vertex_count(line+2) != 3)  {
                g_free(line);
                d3v_error(_("All faces must be triangles."));
                return -1;
            }
            ++ info->face_count;
        }
    }

    if (info->vertex_count)
        info->have_texture = true;
    if (info->normal_count)
        info->have_normal = true;

    return 0;
}


static int
parse_vertex(vec3 *vertex, char *rep)
{
    char **split;
    unsigned size;
    split = g_strsplit(rep, " ", 0);
    size = d3v_strsplit_size(split);
    if (size != 3) {
        g_strfreev(split);
        return -D3V_ERR_WAVEFRONT_PARSE_VERTEX;
    }
    vertex->x = g_ascii_strtod(split[0], NULL);
    vertex->y = g_ascii_strtod(split[1], NULL);
    vertex->z = g_ascii_strtod(split[2], NULL);
    g_strfreev(split);
    return 0;
}

static int
parse_tex_coord(vec2 *tex_coord, char const *rep)
{
    char **split = g_strsplit(rep, " ", 0);
    unsigned size = d3v_strsplit_size(split);

    if (size != 2) {
        g_strfreev(split);
        return -D3V_ERR_WAVEFRONT_PARSE_TEXCOORD;
    }
    tex_coord->x = g_ascii_strtod(split[0], NULL);
    tex_coord->y = g_ascii_strtod(split[1], NULL);
    tex_coord->y = 1 - tex_coord->y;

    g_strfreev(split);
    return 0;
}

static int
parse_normal(vec3 *normal, char const *rep)
{
    char **split = g_strsplit(rep, " ", 0);
    unsigned size = d3v_strsplit_size(split);

    if (size != 3) {
        g_strfreev(split);
        return -D3V_ERR_WAVEFRONT_PARSE_NORMAL;
    }
    normal->x = g_ascii_strtod(split[0], NULL);
    normal->y = g_ascii_strtod(split[1], NULL);
    normal->z = g_ascii_strtod(split[2], NULL);
    
    g_strfreev(split);
    return 0;
}

static int
parse_vertex_elements(char *indices, d3v_vertex *vertex_out,
                      vec3 *vertex, vec2 *tex_coord, vec3 *normal)
{
    char **split;
    unsigned size;
    split = g_strsplit(indices, "/", 0);
    size = d3v_strsplit_size(split);
    if (!size)
        return -D3V_ERR_WAVEFRONT_PARSE_VERTEX_INDEX;
    {
        int i = atoi(split[0])-1;
        vertex_out->x = vertex[i].x;
        vertex_out->y = vertex[i].y;
        vertex_out->z = vertex[i].z;
    }
    
    if (size >= 2 && strcmp(g_strstrip(split[1]), "") != 0) {
        int i = atoi(split[1])-1;
        vertex_out->s = tex_coord[i].x;
        vertex_out->t = tex_coord[i].y;

    }
    if (size == 3) {
        int i = atoi(split[2])-1;
        vertex_out->nx = tex_coord[i].x;
        vertex_out->ny = tex_coord[i].y;
        vertex_out->nz = tex_coord[i].y;
    }

    g_strfreev(split);
    return 0;
}

static int
parse_face(char *face, d3v_vertex *vertices, unsigned i,
           vec3 *vertex, vec2 *tex_coord, vec3 *normal)
{
    int err = 0;
    char **split;
    unsigned size;

    split = g_strsplit(face, " ", 0);
    size = d3v_strsplit_size(split);

    if (size != 3) {
        g_strfreev(split);
        return -D3V_ERR_WAVEFRONT_PARSE_FACE;
    }
    err |= parse_vertex_elements(
        split[0], vertices+i, vertex, tex_coord, normal);
    err |= parse_vertex_elements(
        split[1], vertices+i+1, vertex, tex_coord, normal);
    err |= parse_vertex_elements(
        split[2], vertices+i+2, vertex, tex_coord, normal);
    
    g_strfreev(split);
    return err;
}

#define LINE_IS_COMMENT(line) ((line)[0] == '#')

static int
load_objects(d3v_model *model, GIOChannel *chan)
{
    int err = 0;
    d3v_model_scaninfo info;
    
    err = scan_wavefront_file(chan, &info);
    if (err) {
        fprintf(stderr, "scanning %s error.", model->file_path);
        return -1;
    }
    g_io_channel_seek_position(chan, 0, G_SEEK_SET, NULL);
    
    vec3 *vertex, *normal;
    vec2 *tex_coord;

    ALLOC_ARRAY(vertex, info.vertex_count);
    ALLOC_ARRAY(normal, info.normal_count);
    ALLOC_ARRAY(tex_coord, info.tex_coord_count);
    ALLOC_ARRAY0(model->vertices, 3*info.face_count);
    
    model->vertices_count = 3*info.face_count;
    model->have_texture = info.have_texture;
    model->have_normal = info.have_normal;
    printf(_("scanned %d faces\n"), info.face_count);
    
    unsigned v, vn, vt, f;
    v = vn = vt = f = 0;
    
    char *line = NULL;
    for (; !(err = d3v_readline(chan, &line)); g_free(line)) {
        char *type_str, *value;
        
        if (LINE_IS_COMMENT(line))
            continue;
        if (strlen(line) < 2) {
            err = -D3V_ERR_WAVEFRONT_INVALID_LINE;
            g_free(line);
            break;
        }
        
        type_str = g_strndup(line, 2);
        value = g_strstrip(g_strdup(line+2));
        err = 0;
        switch (get_object_type(type_str)) {
        case OBJECT_VERTEX:
            err = parse_vertex(&vertex[v++], value);
            break;
        case OBJECT_TEX_COORD:
            err = parse_tex_coord(&tex_coord[vt++], value);
            break;
        case OBJECT_NORMAL:
            err = parse_normal(&normal[vn++], value);
            break;
        case OBJECT_FACE:
            err = parse_face(value, model->vertices, 3*f++,
                             vertex, tex_coord, normal);
            break;
        case OBJECT_OBJECT:
            printf(_("parsing object \"%s\".\n"), value);
            break;
        default:
            d3v_warning(_("unexpected object type '%s'.\n"), type_str);
            break;
        }
        g_free(type_str);
        g_free(value);
        if (err) {
            printf("parse error: %s\n", d3v_errmsg(err));
            break;
        }
    }
    printf(_("parsed %d faces\n"), f);
    g_free(vertex);
    g_free(normal);
    g_free(tex_coord);
    if (err == 1)
        err = 0;
    return err;
}

static int
make_vao(d3v_model *model, d3v_program *program)
{
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);
    
    glGenBuffers(1, &model->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 ARRAY_BUF_SIZE(model->vertices, model->vertices_count),
                 model->vertices, GL_STATIC_DRAW);

    int p = program->position_location;
    glEnableVertexAttribArray(p);
    glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, sizeof(d3v_vertex), NULL);

    if (model->have_normal) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(d3v_vertex),
                              (GLvoid*) G_STRUCT_OFFSET(d3v_vertex, nx));
    }
    if (model->have_texture) {
        int t = program->texcoord_location;
        glEnableVertexAttribArray(t);
        glVertexAttribPointer(t, 2, GL_FLOAT, GL_FALSE, sizeof(d3v_vertex),
                              (GLvoid*) G_STRUCT_OFFSET(d3v_vertex, s));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return 0;
}

d3v_model *d3v_model_new_from_wavefront(char const *file_path,
                                        char const *model_name,
                                        d3v_program *program)
{
    d3v_model *model = g_malloc(sizeof*model);
    if (d3v_model_init(model, file_path, model_name, program) < 0) {
        g_free(model);
        return NULL;
    }
    return model;
}

int
d3v_model_init(d3v_model *model,
               char const *file_path,
               char const *model_name,
               d3v_program *program)
{
    int err = 0;
    memset(model, 0, sizeof*model);
    
    model->model_name = g_strdup(model_name);
    model->file_path = g_strdup(file_path);

    GIOChannel *chan = g_io_channel_new_file(file_path, "r", NULL);
    if (chan == NULL) {
        perror(file_path);
        return -1;
    }
    if ((err = load_objects(model, chan)) < 0) {
        d3v_model_free(model);
        g_io_channel_unref(chan);
        return err;
    }
    g_io_channel_unref(chan);

    if ((err = make_vao(model, program)) < 0) {
        d3v_model_free(model);
        return err;
    }
    return err;
}

void d3v_model_draw(d3v_model *model, d3v_program *program)
{
    glBindVertexArray(model->vao);
    glDrawArrays(GL_TRIANGLES, 0, model->vertices_count);
}

void d3v_model_free(d3v_model *model)
{
    glDeleteBuffers(1, &model->vao);
    g_free(model->model_name);
    g_free(model->file_path);
    g_free(model->vertices);
    memset(model, 0, sizeof*model);
}

void d3v_model_delete(d3v_model *model)
{
    d3v_model_free(model);
    g_free(model);
}

void d3v_model_dump(d3v_model *model)
{
    for (unsigned i = 0; i < model->vertices_count; ++i)
    {
        d3v_vertex *v = &model->vertices[i];
        printf("v%*u : %f %f %f\n", 5, i, v->x, v->y, v->z);
    }
}
