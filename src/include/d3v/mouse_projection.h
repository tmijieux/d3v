#ifndef D3V_MOUSE_PROJECTION_H
#define D3V_MOUSE_PROJECTION_H

#include "math_util.h"


int d3v_mouse_projection(vec3 *pos, int x, int y,
                         float proj[restrict 16],
                         float view[restrict 16]);

#endif // D3V_MOUSE_PROJECTION_H
