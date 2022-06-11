
#pragma once
#include <flecs.h>

void movable_set_vel(ecs_world_t *world, ecs_entity_t entity, Vector2 vel);
void movable_add(ecs_world_t *world, ecs_entity_t entity);

void transform_set_pos(ecs_world_t *world, ecs_entity_t entity, Vector2 pos);
void transform_add(ecs_world_t *world, ecs_entity_t entity);

void collidable_add(ecs_world_t *world, ecs_id_t entity);
void collidable_set(ecs_world_t *world, ecs_id_t entity, int width, int height);