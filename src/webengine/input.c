#include "engine.h"
#include "include/webengine.h"

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

#include <stdio.h>

#define INPUT_LERP_SPEED 100

void we_input_init() {
    ecs_world_t *world = we_get_world();
    WE_C(we_input);
    ecs_singleton_add(world, we_input);
    ecs_singleton_set(world, we_input, {.axis = (Vector2){0, 0}});
}

void we_update_input() {

    Vector2 n_axis = {0, 0};
    if (IsKeyDown(KEY_A)) {
        n_axis.x -= 1;
    }
    if (IsKeyDown(KEY_D)) {
        n_axis.x += 1;
    }
    if (IsKeyDown(KEY_W)) {
        n_axis.y -= 1;
    }
    if (IsKeyDown(KEY_S)) {
        n_axis.y += 1;
    }

    WE_C(we_input);
    const we_input *inp = ecs_singleton_get(we_get_world(), we_input);

    Vector2 axis = inp->axis;

    // axis = we_v2_lerp(axis, n_axis, INPUT_LERP_SPEED * GetFrameTime());
    axis = Vector2Lerp(axis, n_axis, INPUT_LERP_SPEED * 0.0166);

    ecs_singleton_set(we_get_world(), we_input, {.axis = axis});
}

Vector2 we_get_axis() {
    WE_C(we_input);
    const we_input *inp = ecs_singleton_get(we_get_world(), we_input);
    return inp->axis;
}

bool we_is_input() {
    WE_C(we_input);
    const we_input *inp = ecs_singleton_get(we_get_world(), we_input);
    Vector2 axis = inp->axis;

    return axis.x != 0 || axis.y != 0;
}