#ifndef D3V_MATH_UTIL_H
#define D3V_MATH_UTIL_H

#include <stdbool.h>
#define __USE_XOPEN 1
#include <math.h>

typedef struct vec2 {
    float x, y;
} vec2;
    
typedef struct vec3 {
    float x, y, z;
} vec3;
    
typedef struct vec4 {
    float x, y, z, w;
} vec4;

void vec3_normalize(vec3 *v);

double deg2rad(double deg);
double rad2deg(double radian);
#define deg2rad(deg) (M_PI * (deg) / 180.)
#define rad2deg(rad) (180*(rad)/M_PI)

double angle_rotation_pingouin(const vec3 *src, const vec3 *dst);

void matrix_rotation(float *out, float angle, vec4 direction);
void matrix_translation(float *out, vec3 t);
void matrix_scale(float *out, vec3 scale);

void matrix_vector_multiply(float *M, vec4 *V);
void matrix_multiply(float M1[restrict 16],
                     float M2[restrict 16],
                     float OUT[restrict 16]);

void matrix_identity(float *M);

void matrix_fov_projection(
    float res[], float fov, float aspect, float near, float far);

#define SQUARE(x) ((x)*(x))
#define SCALAR_MULT4(V, s)                      \
    do {                                        \
        (V).x *= (s);                           \
        (V).y *= (s);                           \
        (V).z *= (s);                           \
        (V).w *= (s);                           \
    } while (0)

#define SCALAR_MULT4_3(V, s)                    \
    do {                                        \
        (V).x *= (s);                           \
        (V).y *= (s);                           \
        (V).z *= (s);                           \
    } while (0)


#define ADD4(V, W)                              \
    do {                                        \
        (V).x += (W).x;                         \
        (V).y += (W).y;                         \
        (V).z += (W).z;                         \
        (V).w += (W).w;                         \
    } while (0)

#define ADD4_3(V, W)                            \
    do {                                        \
        (V).x += (W).x;                         \
        (V).y += (W).y;                         \
        (V).z += (W).z;                         \
    } while (0)

void vec4_diff(vec4 *a, vec4 *b, vec4 *c);
float vec4_norm(vec4 *a);
void vec4_normalize(vec4 *a);
void matrix_look_at(float *res, vec4 eye, vec4 center, vec4 up);

void vec4_print(vec4 *v);
void mat4_print(float *mat);

#endif //D3V_MATH_UTIL_H
