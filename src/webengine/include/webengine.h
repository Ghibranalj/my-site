#pragma once
#include "flecs.h"

typedef void (*we_update_callback)(float time);
typedef void (*we_callback)();
// typedef void (*we_init_callback)(ecs_world_t *world);

typedef struct {
    // CALLBACKS
    we_update_callback on_update;
    we_callback on_init;
    we_callback on_destroy;

    // ecs
    ecs_world_t *world;

    // DATA
    int width, height, fps;
    char *title;
} we_game_t;

void we_create_and_start(we_game_t *game);

ecs_world_t *we_get_world();

#define WE_C(c) ECS_COMPONENT(we_get_world(), c)

// engine provided components

typedef struct {
    Texture2D texture;
} we_sprite;

we_sprite *we_load_sprite(const char *path);

typedef struct {
    float x, y;
} we_position;

typedef struct {
    Texture2D texture;

} we_level;