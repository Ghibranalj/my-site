
#pragma once
#include <flecs.h>

void we_movable_set_vel(ecs_world_t *world, ecs_entity_t entity, Vector2 vel);
void we_movable_add(ecs_world_t *world, ecs_entity_t entity);
void we_movable_add_vel(ecs_world_t *world, ecs_entity_t entity, Vector2 vel);

void we_transform_set_pos(ecs_world_t *world, ecs_entity_t entity, Vector2 pos);
void we_transform_add(ecs_world_t *world, ecs_entity_t entity);

void we_collidable_add(ecs_world_t *world, ecs_id_t entity);
void we_collidable_set(ecs_world_t *world, ecs_id_t entity, int width,
                       int height);