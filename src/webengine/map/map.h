#pragma once

#include <flecs.h>

void draw_map_system(ecs_iter_t *it);

void on_delete_map(ecs_iter_t *it);

void init_map();
