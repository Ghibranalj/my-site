#include <emscripten/emscripten.h>
#include <stddef.h>
#include <stdio.h>

#include "tmx.h"
#include <flecs.h>
#include <raylib.h>

#include "engine.h"
#include "include/webengine.h"

// global variable declarations
we_game_t *we_game = NULL;
ecs_world_t *we_world = NULL;

// local function declarations
void we_init(void);
void we_update(void);
void we_destroy(void);
void we_ecs_init(void);

void we_ecs_init_systems();
void we_ecs_init_triggers();

// exported functions
void we_create_and_start(we_game_t *game) {
    we_game = game;

    we_init();

    emscripten_set_main_loop(we_update, 0, 1);
    we_destroy();
}

ecs_world_t *we_get_world() {
    return we_world;
}

// local functions

Camera2D we_camera;

void we_init() {

    we_ecs_init();

    SetTargetFPS(we_game->fps);
    InitAudioDevice();
    InitWindow(we_game->width, we_game->height, we_game->title);
    we_init_map();

    we_game->on_init();
    we_camera = (Camera2D){
        .offset = {0, 0},
        .zoom = 7,
        .rotation = 0,
        .target = {64, 160},
    };
}

// must be called before using any components in any way
#define _WE_RAC()                                                              \
    WE_C(we_sprite);                                                           \
    WE_C(we_position)

void we_ecs_init() {
    we_world = ecs_init();

    // register all systems
    we_ecs_init_systems();

    // register all triggers
    we_ecs_init_triggers();
}

void we_ecs_init_systems() {
    _WE_RAC();
    ECS_SYSTEM(we_world, we_draw_system, EcsOnUpdate, we_sprite, we_position);
}

void we_ecs_init_triggers() {
    _WE_RAC();
    ecs_trigger_init(we_world,
                     &(ecs_trigger_desc_t){.term = {ecs_id(we_sprite)},
                                           .events = {EcsOnRemove, EcsUnSet},
                                           .callback = we_on_delete_sprite});
}

void we_update() {
    float delta = GetFrameTime();
    we_game->on_update(delta);
    BeginMode2D(we_camera);
    // BeginDrawing();
    we_draw_map();

    ecs_progress(we_world, delta);
    // EndDrawing();

    if (IsKeyPressed(KEY_A))
        we_camera.target.x -= 5;
    if (IsKeyPressed(KEY_W))
        we_camera.target.y -= 5;
    if (IsKeyPressed(KEY_D))
        we_camera.target.x += 5;
    if (IsKeyPressed(KEY_S))
        we_camera.target.y += 5;
    EndMode2D();
}

void we_destroy() {
    we_game->on_destroy();
    ecs_fini(we_world);
}
