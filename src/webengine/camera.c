#include "../include/logging/logging.h"
#include <stdlib.h>

#include "engine.h"
#include "include/webengine.h"
#include <raylib.h>
#include <raymath.h>

#include <flecs.h>

float lerp(float pos, float target, float v);

void init_camera() {

    ecs_world_t *world = get_world();
    C(camera);

    Camera2D *cam = malloc(sizeof(Camera2D));
    cam->target = (Vector2){0, 0};
    cam->offset = (Vector2){0, 0};
    cam->rotation = 0;
    cam->zoom = 1;

    ecs_singleton_add(world, camera);
    ecs_singleton_set(world, camera, {.camera = cam});
}

void destoy_camera() {
    ecs_world_t *world = get_world();
    C(camera);

    const camera *cam = ecs_singleton_get(world, camera);
    free(cam->camera);
}

Camera2D *get_camera() {
    C(camera);
    Camera2D *cam = ecs_singleton_get(get_world(), camera)->camera;
    return cam;
}

void center_camera(float x, float y) {
    Camera2D *cam = get_camera();

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    cam->offset.x = (w / 2);
    cam->offset.y = (h / 2);
    cam->target.x = x;
    cam->target.y = y;
}

void lerp_camera(float x, float y, float speed) {
    Camera2D *cam = get_camera();
    float delta = GetFrameTime();

    Vector2 t = Vector2Lerp(cam->target, (Vector2){x, y}, delta * speed);
    center_camera(t.x, t.y);
}

void zoom_camera(float zoom) {
    Camera2D *cam = get_camera();
    cam->zoom = zoom;
}
