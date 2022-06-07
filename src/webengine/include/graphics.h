
#pragma once
#include <flecs.h>

void we_spitesheet_set(ecs_world_t *world, ecs_entity_t entity,
                       char *path_to_texture, int width, int height,
                       int offset);

void we_spritesheet_add(ecs_world_t *world, ecs_entity_t entity);