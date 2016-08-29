#ifndef D3V_SHADER_H
#define D3V_SHADER_H

typedef struct d3v_shader_ {
    GLuint id;
} d3v_shader;

typedef struct d3v_program_ {
    GLuint id;

    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;
    GLuint position_location;
    GLuint texcoord_location;
} d3v_program;

d3v_shader *
d3v_shader_new_from_resource(char const *resource_path, GLint vertex_type);
int d3v_shader_init(d3v_shader *shader, char const *program, GLint vertex_type);
int d3v_program_init(d3v_program *program, ...);


#endif // D3V_SHADER_H
