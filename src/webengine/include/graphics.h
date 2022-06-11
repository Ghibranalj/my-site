
#pragma once
#include <flecs.h>
#include <tmx.h>

void spitesheet_set(ecs_world_t *world, ecs_entity_t entity,
                    char *path_to_texture, int width, int height, int offset);

void spritesheet_add(ecs_world_t *world, ecs_entity_t entity);

typedef struct {
    int *frames;
    int num_frames;
    float speed;
    bool disabled;

    // internal use
    int index;
    float time_since_last_frame;
} animation, oneway_anim;

typedef struct {
    int **animations;
    int *length_of_animations;
    int len;
    int index;
} anim_manager;

typedef struct {
    tmx_map *map;
} map;

int **animations(int len);

int *anim_frames(int num, int *frames_arr);

void change_anim_mngr_index(ecs_world_t *world, ecs_id_t entity, int index);