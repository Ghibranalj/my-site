#include <stdlib.h>

#include "engine.h"
#include "include/webengine.h"

#include <flecs.h>

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
