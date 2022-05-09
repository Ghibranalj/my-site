#include <emscripten/emscripten.h>
#include <stddef.h>
#include <stdio.h>

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

// exported functions
void we_create(we_game_t *game) {
    we_game = game;
    we_init();
    emscripten_set_main_loop(we_update, 0, 1);
    we_destroy();
}

ecs_world_t *we_get_world() {
    return we_world;
}

// local functions

void we_init() {
    printf("Starting we\n");

    we_ecs_init();

    SetTargetFPS(we_game->fps);
    InitAudioDevice();
    InitWindow(we_game->width, we_game->height, we_game->title);
    we_game->on_init(we_world);
}

void we_ecs_init() {
    we_world = ecs_init();

    // register all components
    WE_RC(we_sprite);
    WE_RC(we_position);
    // register all systems
    ECS_SYSTEM(we_world, we_draw_system, EcsOnUpdate, we_sprite, we_position);

    // register all triggers
    ecs_trigger_init(we_world,
                     &(ecs_trigger_desc_t){.term = {ecs_id(we_sprite)},
                                           .events = {EcsOnRemove, EcsUnSet},
                                           .callback = we_on_delete_sprite});
}

void we_update() {
    float delta = GetFrameTime();
    we_game->on_update(delta);
    BeginDrawing();
    ClearBackground(RAYWHITE);
    ecs_progress(we_world, delta);
    EndDrawing();
}

void we_destroy() {
    we_game->on_destroy();
    ecs_fini(we_world);
}
