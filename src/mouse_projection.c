#include <math.h>

#include <GL/freeglut.h>
#include <GL/glu.h>

#include "d3v/math_util.h"
#include "d3v/mouse_projection.h"

int d3v_mouse_projection(vec3 *pos, int x, int y,
                         float projf[restrict 16],
                         float viewf[restrict 16])
{
    GLdouble ox = 0.0, oy = 0.0, oz = 0.0;
    GLint viewport[4];
    GLfloat wx = x, wy, wz;

    glGetIntegerv(GL_VIEWPORT, viewport);
    wy = y =  viewport[3] - y;

    double projd[16], viewd[16];
    for (int i = 0; i < 16; ++i) {
        projd[i] = projf[i];
        viewd[i] = viewf[i];
    }

    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
    gluUnProject(wx, wy, wz, viewd, projd, viewport, &ox, &oy, &oz);
    *pos = (vec3) { ox, oy, oz };
    return 1;
}
