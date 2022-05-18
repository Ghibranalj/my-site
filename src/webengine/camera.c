#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "include/webengine.h"
#include <raylib.h>
#include <raymath.h>

#include <flecs.h>

float lerp(float pos, float target, float v);

void we_init_camera() {

    ecs_world_t *world = we_get_world();
    WE_C(we_camera);

    Camera2D *cam = malloc(sizeof(Camera2D));
    cam->target = (Vector2){0, 0};
    cam->offset = (Vector2){0, 0};
    cam->rotation = 0;
    cam->zoom = 1;

    ecs_singleton_add(world, we_camera);
    ecs_singleton_set(world, we_camera, {.camera = cam});
}

void we_destoy_camera() {
    ecs_world_t *world = we_get_world();
    WE_C(we_camera);

    const we_camera *cam = ecs_singleton_get(world, we_camera);
    free(cam->camera);
}

Camera2D *we_get_camera() {
    WE_C(we_camera);
    Camera2D *cam = ecs_singleton_get(we_get_world(), we_camera)->camera;
    return cam;
}

void we_center_camera(float x, float y) {
    Camera2D *cam = we_get_camera();

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    cam->offset.x = (w / 2);
    cam->offset.y = (h / 2);
    cam->target.x = x;
    cam->target.y = y;
}

void we_lerp_camera(float x, float y, float speed) {
    Camera2D *cam = we_get_camera();
    float delta = GetFrameTime();

    Vector2 t = we_v2_lerp(cam->target, (Vector2){x, y}, delta * speed);
    we_center_camera(t.x, t.y);
}

void we_zoom_camera(float zoom) {
    Camera2D *cam = we_get_camera();
    cam->zoom = zoom;
}
