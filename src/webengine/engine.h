#pragma once
#include "flecs.h"

#include "raylib.h"

#include "ferox.h"

#define COLLISION_LAYER_NAME "_we_collision"

#define PHYSICS_WORLD_WIDTH (frNumberPixelsToMeters(800))
#define PHYSICS_WORLD_HEIGHT (frNumberPixelsToMeters(500))
// system
void we_draw_system(ecs_iter_t *it);
void we_on_delete_sprite(ecs_iter_t *it);

void we_draw_map_system(ecs_iter_t *it);
void we_on_delete_map(ecs_iter_t *it);

void we_draw_spritesheet(ecs_iter_t *it);
void we_on_delete_spritesheet(ecs_iter_t *it);

void we_animate_system(ecs_iter_t *it);

void we_update_system(ecs_iter_t *it);
void we_oneway_anim_system(ecs_iter_t *it);

void we_script_system(ecs_iter_t *it);

void we_on_delete_anim(ecs_iter_t *it);
void we_on_delete_oneway_anim(ecs_iter_t *it);

void we_on_mngr_set(ecs_iter_t *it);

void we_init_map();

void we_init_camera();
void we_destoy_camera();

void we_input_init();
void we_update_input();

// Physics Engine
void we_physics_init();
void we_physics_update();
void we_physics_draw_all_bodies();
void we_physics_add_to_world(frBody *body);

void we_physics_body_system(ecs_iter_t *it);
void we_physics_body_on_set(ecs_iter_t *it);

void we_coll_map_on_set(ecs_iter_t *it);

// must be called before using any components in any way
#define _WE_RAC()                                                              \
    WE_C(we_sprite);                                                           \
    WE_C(we_transform);                                                        \
    WE_C(we_map);                                                              \
    WE_C(we_spritesheet);                                                      \
    WE_C(we_animation);                                                        \
    WE_C(we_oneway_anim);                                                      \
    WE_C(we_script);                                                           \
    WE_C(we_anim_manager);                                                     \
    WE_C(we_coll_map);                                                         \
    WE_C(we_physics_body)

typedef struct {
    Vector2 axis;
} we_input;

typedef struct {
    Camera2D *camera;
} we_camera;
