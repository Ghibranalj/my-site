#pragma once

#include <flecs.h>

void we_draw_map_system(ecs_iter_t *it);

void we_on_delete_map(ecs_iter_t *it);

void we_init_map();
