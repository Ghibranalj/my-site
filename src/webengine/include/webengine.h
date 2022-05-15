#pragma once
#include <flecs.h>
#include <raylib.h>
#include <stdbool.h>
#include <tmx.h>

typedef void (*we_update_callback)(float time);
typedef void (*we_callback)();

typedef void (*we_script_callback)(float time, ecs_entity_t entity,
                                   ecs_world_t *t_world);
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

typedef struct {
    int *frames;
    int num_frames;
    float speed;
    bool disabled;

    // internal use;
    int index;
    float time_since_last_frame;
} we_animation, we_oneway_anim;

int *we_anim_frames(int num, int *frames_arr);

typedef struct {
    we_script_callback on_update;
} we_script;

#define WE_C(c) ECS_COMPONENT(we_get_world(), c)

// must be called before using any components in any way
#define _WE_RAC()                                                              \
    WE_C(we_sprite);                                                           \
    WE_C(we_transform);                                                        \
    WE_C(we_map);                                                              \
    WE_C(we_spritesheet);                                                      \
    WE_C(we_animation);                                                        \
    WE_C(we_oneway_anim);                                                      \
    WE_C(we_script)
