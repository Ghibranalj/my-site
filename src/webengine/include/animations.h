#pragma once

#include <flecs.h>

typedef struct {
    int *frames;
    int num_frames;
    float speed;
    bool disabled;

    // internal use
    int index;
    float time_since_last_frame;
} we_animation, we_oneway_anim;

typedef struct {
    int **animations;
    int *length_of_animations;
    int len;
    int index;
} we_anim_manager;

typedef struct {
    tmx_map *map;
} we_map;

int **we_animations(int len);

int *we_anim_frames(int num, int *frames_arr);

void we_change_anim_mngr_index(ecs_world_t *world, ecs_id_t entity, int index);