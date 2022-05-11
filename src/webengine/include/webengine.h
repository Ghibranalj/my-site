#pragma once
#include <flecs.h>
#include <raylib.h>
#include <tmx.h>

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

typedef struct {
    Texture2D texture;
    int width, height;
    int offset;
} we_spritesheet;

typedef struct {
    Vector2 position;
} we_transform;

// automatically prefixed
#define WE_MAP_LOCATION "resources/tmx/tiled/"

typedef struct {
    tmx_map *map;
} we_map;