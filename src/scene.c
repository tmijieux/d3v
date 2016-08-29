#include <string.h>
#include "d3v/scene.h"

int
d3v_scene_init(d3v_scene *scene)
{
    vec4 look = { 0., 0., 0., 1. };
    memset(scene, 0, sizeof *scene);
    d3v_camera_init(&scene->camera, look, 1., 0., 0.);
    return 0;
}

void
d3v_scene_free(d3v_scene *scene)
{
    for (int i = 0; i < scene->object_count; i++)
	d3v_object_free(scene->object[i]);
    
    for (int i = 0; i < scene->model_count; i++)
	d3v_model_free(scene->model[i]);
    
    for (int i = 0; i < scene->texture_count; i++)
	d3v_texture_free(scene->texture[i]);

    d3v_camera_free(&scene->camera);
    d3v_light_free(&scene->light);
    
    memset(scene, 0, sizeof *scene);
}

int d3v_scene_add_model(d3v_scene *scene, d3v_model *model)
{
    if (scene->model_count < MAX_MODEL) {
        scene->model[scene->model_count++] = model;
        return 0;
    }
    return -1;
}

int d3v_scene_add_object(d3v_scene *scene, d3v_object *object)
{
    if (scene->object_count < MAX_OBJECT) {
        scene->object[scene->object_count++] = object;
        return 0;
    }
    return -1;
}

int d3v_scene_add_texture(d3v_scene *scene, d3v_texture *texture)
{
    if (scene->texture_count < MAX_TEXTURE) {
        scene->texture[scene->texture_count++] = texture;
        return 0;
    }
    return -1;
}

