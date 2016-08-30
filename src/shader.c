#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v/shader.h"

int d3v_shader_init(d3v_shader *shader, char const *program, GLint vertex_type)
{
    shader->id = glCreateShader(vertex_type);
    glShaderSource(shader->id, 1, &program, NULL);
    glCompileShader(shader->id);

    int status;
    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_len;
        glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = g_malloc(log_len + 1);
        glGetShaderInfoLog(shader->id, log_len, NULL, buffer);
        printf("shader compilation error: %s\n", buffer);
        g_free(buffer);
        glDeleteShader(shader->id);
        shader->id = 0;
    }
    return !!shader->id;
}

d3v_shader *
d3v_shader_new_from_resource(char const *resource_path, GLint vertex_type)
{
    GBytes *source;
    d3v_shader *shader;

    source = g_resources_lookup_data(resource_path, 0, NULL);
    shader = g_malloc(sizeof*shader);
    d3v_shader_init(shader, g_bytes_get_data(source, NULL), vertex_type);
    g_bytes_unref(source);

    return shader;
}

int d3v_program_init(d3v_program *program, ...)
{
    va_list ap;
    d3v_shader *shader;

    va_start(ap, program);
    memset(program, 0, sizeof*program);

    program->id = glCreateProgram();
    while ((shader = va_arg(ap, d3v_shader*)) != NULL)
        glAttachShader(program->id, shader->id);

    glLinkProgram(program->id);
    va_end(ap);

    int status = 0;
    glGetProgramiv(program->id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_len = 0;
        glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = g_malloc(log_len + 1);
        glGetProgramInfoLog(program->id, log_len, NULL, buffer);
        printf("shader linking error: %s\n", buffer);
        g_free(buffer);
        glDeleteProgram(program->id);
        program->id = 0;
    }
    if (program->id) {
        program->model_location = glGetUniformLocation(program->id, "model");
        program->view_location = glGetUniformLocation(program->id, "view");
        program->proj_location = glGetUniformLocation(program->id, "projection");
        program->sampler_location = glGetUniformLocation(program->id, "tex");

        program->position_location = glGetAttribLocation(program->id, "position");
        program->texcoord_location = glGetAttribLocation(program->id, "TexCoord");
        glUseProgram(program->id);
    }
    return !!program->id;
}
