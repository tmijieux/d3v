#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "d3v/math_util.h"

void vec3_normalize(vec3 *v)
{
    double n = sqrt(v->x*v->x+v->y*v->y+v->z*v->z);
    v->x /= n;  v->y /= n;  v->z /= n;
}

#undef deg2rad
#undef rad2deg

double deg2rad(double deg)
{
    return M_PI * deg / 180.;
}

double rad2deg(double radian)
{
    return radian * 180. / M_PI;
}

void matrix_scale(float *i, vec3 s)
{
    i[0] = s.x; i[4] = 0.;  i[8]  = 0.;  i[12] = 0.;
    i[1] = 0.;  i[5] = s.y; i[9]  = 0.;  i[13] = 0.;
    i[2] = 0.;  i[6] = 0.;  i[10] = s.z; i[14] = 0.;
    i[3] = 0.;  i[7] = 0.;  i[11] = 0.;  i[15] = 1.;
}

void matrix_rotation(float *m, float angle, vec4 ax)
{
    float co = cos(angle); float co2 = 1.-co; float si = sin(angle);
    float xx = ax.x*ax.x, yy = ax.y*ax.y, zz = ax.z*ax.z;
    float xy = ax.x*ax.y, xz = ax.x*ax.z, yz = ax.y*ax.z;
    
    m[0] = co+xx*co2;     m[4] = xy*co2-ax.z*si;m[8]  = xz*co2+ax.y*si;m[12] = 0.0f;
    m[1] = xy*co2+ax.z*si;m[5] = co+yy*co2;     m[9]  = yz*co2-ax.x*si;m[13] = 0.0f;
    m[2] = xz*co2-ax.y*si;m[6] = yz*co2+ax.x*si;m[10] = co+zz*co2;     m[14] = 0.0f;
    m[3] = 0.0f;          m[7] = 0.0f;          m[11] = 0.0f;          m[15] = 1.0f;
}

void matrix_vector_multiply(float *M, vec4 *V)
{
    vec4 v = *V;
    V->x = M[0]*v.x + M[4]*v.y + M[8] *v.z + M[12]*v.w;
    V->y = M[1]*v.x + M[5]*v.y + M[9] *v.z + M[13]*v.w;
    V->z = M[2]*v.x + M[6]*v.y + M[10]*v.z + M[14]*v.w;
    V->w = M[3]*v.x + M[7]*v.y + M[11]*v.z + M[15]*v.w;
}

void matrix_multiply(float *m1, float *m2, float *res)
{
    memset(res, 0, sizeof*res * 16);
    for (unsigned i = 0; i < 4; ++i)
        for (unsigned k = 0; k < 4; ++k)
            for (unsigned j = 0; j < 4; ++j)
                res[j*4+i] += m1[k*4+i] * m2[j*4+k];
}

void matrix_identity(float *M)
{
    M[0] = 1.; M[4] = 0.; M[8]  = 0.; M[12] = 0.;
    M[1] = 0.; M[5] = 1.; M[9]  = 0.; M[13] = 0.;
    M[2] = 0.; M[6] = 0.; M[10] = 1.; M[14] = 0.;
    M[3] = 0.; M[7] = 0.; M[11] = 0.; M[15] = 1.;
}

void matrix_translation(float *M, vec3 t)
{
    matrix_identity(M);
    M[12] = t.x; M[13] = t.y; M[14] = t.z;
}

void
matrix_fov_projection(float res[], float fov, float aspect, float near, float far)
{
    float depth = near-far;
    float invDepth = 1 / depth;

    memset(res, 0, sizeof*res * 16);
    res[5] = 1.0 / tan(0.5f * fov);
    res[0] = res[5] / aspect;
    res[10] = (far+near) * invDepth;
    res[11] = -1.0;
    res[14] = (2.0*far*near) * invDepth;
}

void vec4_diff(vec4 *restrict a, vec4 *restrict b, vec4 *restrict c)
{
    c->x = a->x - b->x;
    c->y = a->y - b->y;
    c->z = a->z - b->z;
    c->w = a->w - b->w;
}

void vec4_diff3(vec4 *restrict a, vec4 *restrict b, vec4 *restrict c)
{
    c->x = a->x - b->x;
    c->y = a->y - b->y;
    c->z = a->z - b->z;
}

float vec4_norm(vec4 *a)
{
    return sqrt(SQUARE(a->x)+SQUARE(a->y)+SQUARE(a->z)+SQUARE(a->w));
}

float vec4_norm3(vec4 *a)
{
    return sqrt(SQUARE(a->x)+SQUARE(a->y)+SQUARE(a->z));
}

float vec4_dot(vec4 *a, vec4 *b)
{
    return a->x*b->x+a->y*b->y+a->z*b->z+a->w*b->w;
}

float vec4_dot3(vec4 *a, vec4 *b)
{
    return a->x*b->x+a->y*b->y+a->z*b->z;
}

void vec4_normalize3(vec4 *a)
{
    double n = vec4_norm3(a);
    a->x /= n;
    a->y /= n;
    a->z /= n;
}

void vec4_cross3(vec4 *restrict a, vec4 *restrict b, vec4 *restrict c)
{
    c->x = a->y*b->z - a->z*b->y;
    c->y = a->z*b->x - a->x*b->z;
    c->z = a->x*b->y - a->y*b->x;
}

void matrix_look_at(float *L, vec4 eye, vec4 center, vec4 up)
{
    vec4 f, u, s;
    vec4_diff3(&center, &eye, &f);
    vec4_normalize3(&f);
    u = up;
    vec4_normalize3(&u);
    vec4_cross3(&f, &u, &s);
    vec4_normalize3(&s);
    vec4_cross3(&s, &f, &u);
     
    L[0] = s.x;  L[4] = s.y;  L[8]  = s.z;
    L[1] = u.x;  L[5] = u.y;  L[9]  = u.z;
    L[2] = -f.x; L[6] = -f.y; L[10] = -f.z;
    L[3] = 0.0f; L[7] = 0.0f; L[11] = 0.0f;

    L[12] = -vec4_dot3(&s, &eye);
    L[13] = -vec4_dot3(&u, &eye);
    L[14] = vec4_dot3(&f, &eye);
    L[15] = 1.;
}

void vec4_print(vec4 *v)
{
    printf("%g %g %g %g\n", v->x, v->y, v->z, v->w);
}

void mat4_print(float *M)
{
    for (unsigned row = 0; row < 4; ++row) {
        for (unsigned col = 0; col < 4; ++col)
            printf("%f ", M[4*col+row]);
        printf("\n");
    }
}
