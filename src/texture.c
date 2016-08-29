#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <glib/gstdio.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v/texture.h"


/** Chargement d'une image au format JPG **/
static uint8_t*
texture_load_jpeg(const char *file_path, int *width_, int *height_)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned char *line;

    FILE *f = g_fopen(file_path, "r");
    if (f == NULL) {
	perror(file_path);
	exit(EXIT_FAILURE);
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, f);
    jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE) {
	fprintf(stdout, "Erreur: %s doit etre de type RGB\n", file_path);
	exit(EXIT_FAILURE);
    }

    int width = cinfo.image_width;
    int height = cinfo.image_height;

    unsigned char *texture = g_malloc(height * width * 3);

    jpeg_start_decompress(&cinfo);
    line = texture;
    while (cinfo.output_scanline < cinfo.output_height) {
	line = texture + 3 * width * cinfo.output_scanline;
	jpeg_read_scanlines(&cinfo, &line, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(f);

    *width_ = width; *height_ = height;
    return texture;
}

d3v_texture *
d3v_texture_load(const char *path)
{
    d3v_texture *t = g_malloc0(sizeof*t);
    
    glGenTextures(1, &t->tex_id);
    glBindTexture(GL_TEXTURE_2D, t->tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    int width, height;
    t->tex_data = texture_load_jpeg(path, &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		 0, GL_RGB, GL_UNSIGNED_BYTE, t->tex_data);
    return t;
}

GLuint d3v_texture_get_id(d3v_texture *t)
{
    return t->tex_id;
}

void d3v_texture_free(d3v_texture *texture)
{
    glDeleteTextures(1, &texture->tex_id);
    g_free(texture->tex_data);
    memset(texture, 0, sizeof *texture);
}

void d3v_texture_delete(d3v_texture *texture)
{
    d3v_texture_free(texture);
    g_free(texture);
}
