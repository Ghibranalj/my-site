#pragma once
#include "flecs.h"
#include "raylib.h"

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

Vector2 we_v2_lerp(Vector2 a, Vector2 b, float l);

// must be called before using any components in any way
#define _WE_RAC()                                                              \
    WE_C(we_sprite);                                                           \
    WE_C(we_transform);                                                        \
    WE_C(we_map);                                                              \
    WE_C(we_spritesheet);                                                      \
    WE_C(we_animation);                                                        \
    WE_C(we_oneway_anim);                                                      \
    WE_C(we_script);                                                           \
    WE_C(we_anim_manager)

typedef struct {
    Vector2 axis;
} we_input_axis;

typedef struct {
    Camera2D *camera;
} we_camera;
