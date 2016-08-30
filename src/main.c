#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <glib/gi18n.h>

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "d3v.h"

#define WINDOW_X 200
#define WINDOW_Y 200
#define HEIGHT  700
#define WIDTH   700

int window_key;
static d3v_scene *gl_scene;

static void
d3v_set_scene(d3v_scene *scene)
{
    gl_scene = scene;
}

/*
static void draw_basis(void)
{
    glNormal3f(0., 1., 0.);
    glBegin(GL_LINES);

    glColor3f(1., 0., 0.);	// x red
    glVertex3f(0., 0., 0.);
    glVertex3f(1., 0., 0.);

    glColor3f(.0, 1., 0.);	// y green
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 1., 0.);

    glColor3f(0., 0., 1.);	// z blue
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 0., 1.);
    glEnd();
    glColor3f(1., 1., 1.);	// reset color
}
*/

static void draw_cb(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    d3v_camera_update(&gl_scene->camera, gl_scene->object[0]->program);
    for (int i = 0; i < gl_scene->object_count; i++)
	d3v_object_draw(gl_scene->object[i]);
    glutSwapBuffers();
}

static void key_input_cb(unsigned char key, int x, int y)
{
    switch (key) {
    case 27: /* touche ESC */
	glutLeaveMainLoop();
	break;
    }
}

static void
mouse_cb(int button, int state, int x, int y)
{
    gl_scene->xold = x; gl_scene->yold = y;
    switch (button) {
    case GLUT_LEFT_BUTTON:
	switch (state) {
	case GLUT_DOWN: gl_scene->button = 1; break;
	case GLUT_UP:
	    gl_scene->button = 0;
	    vec3 pos;
	    d3v_mouse_projection(&pos, x , y,
                                 gl_scene->camera.projection,
                                 gl_scene->camera.view);
	    printf("%f %f %f\n", pos.x, pos.y, pos.z);
	    break;
	} break;
    case GLUT_RIGHT_BUTTON:
	switch (state) {
	case GLUT_DOWN: gl_scene->button = 2; break;
	case GLUT_UP:   gl_scene->button = 0; break;
	} break;
    case 3: d3v_camera_add_distance(&gl_scene->camera, -0.4); break;
    case 4: d3v_camera_add_distance(&gl_scene->camera, 0.4);  break;
    }
    glutPostRedisplay();
}

static void
mouse_motion_cb(int x, int y)
{
    if (gl_scene->button == 1)// left button -> translate
    {
	d3v_camera_translate(&gl_scene->camera,
                             (double) (gl_scene->xold - x) / 70.,
                             (double) (y - gl_scene->yold) / 70.);
    }
    else if (gl_scene->button == 2) // right button -> rotate
    {
	d3v_camera_rotate(&gl_scene->camera,
                          (double) -(y - gl_scene->yold) / 70,
                          (double) -(x - gl_scene->xold) / 70);
    }

    gl_scene->xold = x;
    gl_scene->yold = y;
    glutPostRedisplay();
}

static void
d3v_glut_init(const char *title, int *argc, char **argv[])
{
    glutInit(argc, *argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);

    window_key = glutCreateWindow(title);

    glutDisplayFunc(&draw_cb);
    glutKeyboardFunc(&key_input_cb);
    glutMouseFunc(&mouse_cb);
    glutMotionFunc(&mouse_motion_cb);
}

static void
d3v_opengl_init(void)
{
    glClearColor(.5, .5, .5, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_TEXTURE_2D);
}

static void
d3v_init(int *argc, char **argv[], char const *window_title)
{
    d3v_glut_init(window_title, argc, argv);
    d3v_opengl_init();
}

static void init_locale(void)
{
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain(PACKAGE);
}

int main(int argc, char *argv[])
{
    init_locale();

    if (argc != 2) {
	fprintf(stderr, _("Usage: %s penguin_count\n"), argv[0]);
	exit(EXIT_FAILURE);
    }
    d3v_init(&argc, &argv, _("penguins"));

    d3v_scene main_scene;
    d3v_model *penguin_model;
    d3v_texture *penguin_texture;
    d3v_shader *vertex_shader, *fragment_shader;
    vertex_shader = d3v_shader_new_from_resource("/org/d3v/vertex.glsl",
                                                 GL_VERTEX_SHADER);
    fragment_shader = d3v_shader_new_from_resource("/org/d3v/fragment.glsl",
                                                   GL_FRAGMENT_SHADER);

    d3v_program prog;
    d3v_program_init(&prog, vertex_shader, fragment_shader, NULL);

    d3v_scene_init(&main_scene);
    penguin_model = d3v_model_new_from_wavefront(
        OBJDIR "penguin.obj", _("penguin"), &prog);
    penguin_texture = d3v_texture_new(TEXDIR "penguin_black.jpg", &prog);

    d3v_scene_add_model(&main_scene, penguin_model);
    d3v_scene_add_texture(&main_scene, penguin_texture);

    //d3v_model_dump(penguin_model);

    int n = atoi(argv[1]);
    if (n > MAX_OBJECT)
        n = MAX_OBJECT;

    vec3 pos = { 0., 0., 0. };
    for (int i = 0; i < n; ++i)
    {
        d3v_object *penguin;

        pos.x = 0.3 * i;
        penguin = d3v_object_create(
            penguin_model, penguin_texture, pos, 90., 0.1);

        d3v_object_set_shademodel(penguin, (i % 2) ? GL_FLAT : GL_SMOOTH);
        d3v_scene_add_object(&main_scene, penguin);
        d3v_object_set_program(penguin, &prog);
    }

    if (n) {
        d3v_set_scene(&main_scene);
        glutMainLoop();
        printf(_("Main loop exited ...\n"));
    }

    d3v_scene_free(&main_scene);
    d3v_model_delete(penguin_model);
    d3v_texture_delete(penguin_texture);

    return EXIT_SUCCESS;
}
