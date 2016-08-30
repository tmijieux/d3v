#ifndef D3V_TEXTURE_H
#define D3V_TEXTURE_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <stdint.h>

typedef struct d3v_texture_ {
    GLuint tex_id;
    uint8_t *tex_data;
} d3v_texture;

int d3v_texture_init(d3v_texture *texture, char const *file_path);
d3v_texture *d3v_texture_new(const char *path);

void d3v_texture_free(d3v_texture *t);
void d3v_texture_delete(d3v_texture *texture);
GLuint d3v_texture_get_id(d3v_texture *t);

#endif // D3V_TEXTURE_H
