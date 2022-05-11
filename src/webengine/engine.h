#pragma once
#include "flecs.h"

// system
void we_draw_system(ecs_iter_t *it);
void we_on_delete_sprite(ecs_iter_t *it);

void we_draw_map_system(ecs_iter_t *it);
void we_on_delete_map(ecs_iter_t *it);

void we_draw_spritesheet(ecs_iter_t *it);
void we_on_delete_spritesheet(ecs_iter_t *it);

void we_init_map();