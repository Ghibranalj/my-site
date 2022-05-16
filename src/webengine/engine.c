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

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(we_update, 0, 1);
#else
    while (!WindowShouldClose()) {
        we_update();
    }
#endif

    we_destroy();
}

ecs_world_t *we_get_world() {
    return we_world;
}

// local functions

Camera2D we_camera;

void we_init() {

    we_camera = (Camera2D){
        .offset = {0, 0},
        .zoom = 4,
        .rotation = 0,
        .target = {100, 100},
    };

    we_ecs_init();

    SetTargetFPS(we_game->fps);
    InitAudioDevice();
    InitWindow(we_game->width, we_game->height, we_game->title);
    we_init_map();

    we_game->on_init();
}

void we_ecs_init() {
    we_world = ecs_init();

    // register all systems
    we_ecs_init_systems();

    // register all triggers
    we_ecs_init_triggers();
}

void we_ecs_init_systems() {
    _WE_RAC();
    ECS_SYSTEM(we_world, we_draw_system, EcsOnUpdate, we_sprite, we_transform);

    ECS_SYSTEM(we_world, we_draw_map_system, EcsPreUpdate, we_map);
    ECS_SYSTEM(we_world, we_draw_spritesheet, EcsOnUpdate, we_spritesheet,
               we_transform);

    ECS_SYSTEM(we_world, we_animate_system, EcsOnUpdate, we_animation,
               we_spritesheet);
    ECS_SYSTEM(we_world, we_oneway_anim_system, EcsOnUpdate, we_oneway_anim,
               we_spritesheet);

    ECS_SYSTEM(we_world, we_script_system, EcsOnUpdate, we_script);
}
//
void we_ecs_init_triggers() {
    _WE_RAC();

    ECS_TRIGGER(we_world, we_on_delete_sprite, EcsOnRemove, we_sprite);

    ECS_TRIGGER(we_world, we_on_delete_map, EcsOnRemove, we_map);

    ECS_TRIGGER(we_world, we_on_delete_anim, EcsOnRemove, we_animation);

    ECS_TRIGGER(we_world, we_on_delete_oneway_anim, EcsOnRemove,
                we_oneway_anim);

    ECS_TRIGGER(we_world, we_on_mngr_set, EcsOnSet, we_anim_manager);

    // TODO add trigger for delete spritesheet
}

void we_update() {

    float delta = GetFrameTime();
    we_game->on_update(delta);

    if (IsKeyDown(KEY_LEFT))
        we_camera.target.x -= 5;
    if (IsKeyDown(KEY_UP))
        we_camera.target.y -= 5;
    if (IsKeyDown(KEY_RIGHT))
        we_camera.target.x += 5;
    if (IsKeyDown(KEY_DOWN))
        we_camera.target.y += 5;

    //
    BeginDrawing();
    //
    BeginMode2D(we_camera);
    ecs_progress(we_world, delta);
    EndMode2D();

    //
    EndDrawing();
}

void we_destroy() {
    we_game->on_destroy();
    ecs_fini(we_world);
}
