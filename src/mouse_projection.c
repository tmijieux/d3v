#include <math.h>

#include <GL/freeglut.h>
#include <GL/glu.h>

#include "d3v/math_util.h"

GLdouble modelview[16] = { 0 };

int d3v_mouse_projection(vec3 *pos, int x, int y)
{
    GLdouble ox = 0.0, oy = 0.0, oz = 0.0;
    GLint viewport[4];
    GLdouble projection[16];
    GLfloat wx = x, wy, wz;
    glGetIntegerv(GL_VIEWPORT, viewport);
    wy = y =  viewport[3] - y;
    
//  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
    gluUnProject(wx, wy, wz,
		 modelview, projection, viewport,
		 &ox, &oy, &oz);
    *pos = (vec3) { ox, oy, oz };
    return 1;
}
