#ifndef D3V_SCENE_H
#define D3V_SCENE_H

#include "d3v/object.h"
#include "d3v/light.h"
#include "d3v/camera.h"

#define MAX_OBJECT  200
#define MAX_TEXTURE 200
#define MAX_MODEL   200

typedef struct d3v_scene_ {
    int button;
    int xold, yold;

    d3v_camera camera;
    d3v_light light;
    
    d3v_object *object[MAX_OBJECT];
    unsigned object_count;
    
    d3v_texture *texture[MAX_TEXTURE];
    unsigned texture_count;
    
    d3v_model *model[MAX_MODEL];
    unsigned model_count;
} d3v_scene;

int d3v_scene_init(d3v_scene *scene);
void d3v_scene_free(d3v_scene *scene);

int d3v_scene_add_model(d3v_scene *scene, d3v_model *model);
int d3v_scene_add_object(d3v_scene *scene, d3v_object *object);
int d3v_scene_add_texture(d3v_scene *scene, d3v_texture *texture);

#endif // D3V_SCENE_H
