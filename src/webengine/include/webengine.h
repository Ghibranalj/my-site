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

typedef void (*update_callback)(float time);
typedef void (*callback)();

typedef void (*script_callback)(float time, ecs_entity_t entity,
                                ecs_world_t *t_world);
// typedef void (*init_callback)(ecs_world_t *world);

typedef struct {
    // CALLBACKS
    update_callback on_update;
    callback on_init;
    callback on_destroy;

    // ecs
    ecs_world_t *world;

    // DATA
    int width, height, fps;
    char *title;
} game_t;

void create_and_start(game_t *game);

ecs_world_t *get_world();

// engine provided components
typedef struct {
    Texture2D texture;
} sprite;

typedef struct {
    Texture2D texture;
    int width, height;
    int offset;
} spritesheet;

typedef struct {
    Vector2 position;
} transform;

typedef struct {
    script_callback on_update;
} script;

typedef struct {
    int width, height;
} collidable;

typedef struct {
    Vector2 velocity;
} movable;

#define C(c) ECS_COMPONENT(get_world(), c)

// input
Vector2 get_axis(float scale);
