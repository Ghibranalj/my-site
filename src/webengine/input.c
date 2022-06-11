#include "engine.h"
#include "include/webengine.h"

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

#include "../include/logging/logging.h"

#define INPUT_LERP_SPEED 100

void input_init() {
    ecs_world_t *world = get_world();
    C(input);
    ecs_singleton_add(world, input);
    ecs_singleton_set(world, input, {.axis = (Vector2){0, 0}});
}

void update_input() {

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

    C(input);
    const input *inp = ecs_singleton_get(get_world(), input);

    Vector2 axis = inp->axis;

    // axis = v2_lerp(axis, n_axis, INPUT_LERP_SPEED * GetFrameTime());
    axis = Vector2Lerp(axis, n_axis, INPUT_LERP_SPEED * GetFrameTime());

    ecs_singleton_set(get_world(), input, {.axis = axis});
}

Vector2 get_axis(float scale) {
    C(input);
    const input *inp = ecs_singleton_get(get_world(), input);
    return Vector2Scale(inp->axis, scale);
}

bool is_input() {
    C(input);
    const input *inp = ecs_singleton_get(get_world(), input);
    Vector2 axis = inp->axis;

    return axis.x != 0 || axis.y != 0;
}