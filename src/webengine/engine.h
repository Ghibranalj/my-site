#pragma once
#include <flecs.h>
#include <raylib.h>

#include "map/map.h"

#define COLLISION_LAYER_NAME "_we_collision"
#define MAX_COLLISION_REC 10240

// system
void draw_system(ecs_iter_t *it);
void on_delete_sprite(ecs_iter_t *it);

void draw_spritesheet(ecs_iter_t *it);
void on_delete_spritesheet(ecs_iter_t *it);

void animate_system(ecs_iter_t *it);

void update_system(ecs_iter_t *it);
void oneway_anim_system(ecs_iter_t *it);

void script_system(ecs_iter_t *it);

void on_delete_anim(ecs_iter_t *it);
void on_delete_oneway_anim(ecs_iter_t *it);

void on_mngr_set(ecs_iter_t *it);

void init_camera();
void destoy_camera();

void input_init();
void update_input();

// must be called before using any components in any way
#define _RAC()                                                                 \
    C(sprite);                                                                 \
    C(transform);                                                              \
    C(map);                                                                    \
    C(spritesheet);                                                            \
    C(animation);                                                              \
    C(oneway_anim);                                                            \
    C(script);                                                                 \
    C(anim_manager);                                                           \
    C(coll_map);                                                               \
    C(movable);                                                                \
    C(collidable)

typedef struct {
    Vector2 axis;
} input;

typedef struct {
    Camera2D *camera;
} camera;
