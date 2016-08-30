#ifndef D3V_ERROR_H
#define D3V_ERROR_H

#define D3V_ERRORS(ERROR)                       \
    ERROR(SUCCESS)                              \
    ERROR(WAVEFRONT_INVALID_LINE)               \
    ERROR(WAVEFRONT_MODEL_UNEXPECTED_OBJECT)    \
    ERROR(WAVEFRONT_PARSE_VERTEX)               \
    ERROR(WAVEFRONT_PARSE_TEXCOORD)             \
    ERROR(WAVEFRONT_PARSE_NORMAL)               \
    ERROR(WAVEFRONT_PARSE_FACE)                 \
    ERROR(WAVEFRONT_PARSE_VERTEX_INDEX)         \
    ERROR(TEXTURE_LOAD)                         \
    ERROR(FILE_BAD_ENCODING)                    \


#define ERROR_TO_ENUM(error)                    \
    D3V_ERR_##error,

enum d3v_error_type {
    D3V_ERRORS(ERROR_TO_ENUM)
    D3V_MAX_ERROR
};

void d3v_warning(char const *message, ...);
void d3v_error(char const *message, ...);
char const* d3v_errmsg(int error);

#endif //ERROR_H
