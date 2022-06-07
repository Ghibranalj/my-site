#pragma once
#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <tmx.h>

#include "camera.h"
#include "collisions.h"
#include "graphics.h"
#include "maps.h"
#include "physics.h"

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

typedef struct {
    we_script_callback on_update;
} we_script;

typedef struct {
    Rectangle bound;
} we_collidable;

typedef struct {
    Vector2 velocity;
} we_movable;

#define WE_C(c) ECS_COMPONENT(we_get_world(), c)

// input
Vector2 we_get_axis(float scale);
