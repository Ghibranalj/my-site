#include <emscripten/emscripten.h>
#include <stddef.h>
#include <stdio.h>

#include "tmx.h"
#include <flecs.h>
#include <raylib.h>

#include "engine.h"
#include "physics/collision.h"
#include "physics/physics.h"

#include "include/webengine.h"

// global variable declarations
game_t *game = NULL;
ecs_world_t *world = NULL;

// local function declarations
void init(void);
void update(void);
void destroy(void);
void engine_ecs_init(void);

void ecs_init_systems();
void ecs_init_triggers();

// exported functions
void create_and_start(game_t *_game) {
    game = _game;

    init();

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update, 0, 1);
#else
    while (!WindowShouldClose()) {
        update();
    }
#endif

    destroy();
}

ecs_world_t *get_world() {
    return world;
}

// local functions

void init() {

    engine_ecs_init();
    SetTargetFPS(game->fps);
    InitAudioDevice();
    InitWindow(game->width, game->height, game->title);
    init_map();
    init_collision();
    init_camera();
    game->on_init();
}

void engine_ecs_init() {
    world = ecs_init();

    // register all systems
    ecs_init_systems();

    // register all triggers
    ecs_init_triggers();

    input_init();
}

void ecs_init_systems() {
    _RAC();
    ECS_SYSTEM(world, draw_system, EcsOnUpdate, sprite, transform);

    ECS_SYSTEM(world, draw_map_system, EcsPreUpdate, map);
    ECS_SYSTEM(world, draw_spritesheet, EcsOnUpdate, spritesheet, transform);

    ECS_SYSTEM(world, animate_system, EcsOnUpdate, animation, spritesheet);
    ECS_SYSTEM(world, oneway_anim_system, EcsOnUpdate, oneway_anim,
               spritesheet);

    ECS_SYSTEM(world, map_coll_system, EcsOnUpdate, collidable, movable,
               transform);

    ECS_SYSTEM(world, movable_system, EcsOnUpdate, movable, transform);

    ECS_SYSTEM(world, script_system, EcsPreUpdate, script);
}
//
void ecs_init_triggers() {
    _RAC();

    ECS_TRIGGER(world, on_delete_sprite, EcsOnRemove, sprite);

    ECS_TRIGGER(world, on_delete_map, EcsOnRemove, map);

    ECS_TRIGGER(world, on_delete_anim, EcsOnRemove, animation);

    ECS_TRIGGER(world, on_delete_oneway_anim, EcsOnRemove, oneway_anim);

    ECS_TRIGGER(world, on_mngr_set, EcsOnSet, anim_manager);

    // TODO add trigger for delete spritesheet
}

void update() {
    float delta = GetFrameTime();

    update_input();

    //
    BeginDrawing();
    //
    C(camera);
    const camera *cam = ecs_singleton_get(world, camera);
    BeginMode2D(*(cam->camera));
    ecs_progress(world, delta);
    game->on_update(delta);
    EndMode2D();

    DrawFPS(10, 10);
    //
    EndDrawing();
}

void destroy() {
    game->on_destroy();
    destoy_camera();
    ecs_fini(world);
}
