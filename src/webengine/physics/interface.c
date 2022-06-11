#include "../include/webengine.h"

/*

Movable

*/

void we_movable_set_vel(ecs_world_t *world, ecs_entity_t entity, Vector2 vel) {
    WE_C(we_movable);

    ecs_set(world, entity, we_movable, {.velocity = vel});
}

void we_movable_add(ecs_world_t *world, ecs_entity_t entity) {
    WE_C(we_movable);
    ecs_add(world, entity, we_movable);
}

void we_movable_add_vel(ecs_world_t *world, ecs_entity_t entity, Vector2 vel) {
    WE_C(we_movable);
    const we_movable *mov = ecs_get(world, entity, we_movable);
    ecs_set(world, entity, we_movable,
            {.velocity = Vector2Add(mov->velocity, vel)});
}

/*

Transform

*/

void we_transform_set_pos(ecs_world_t *world, ecs_entity_t entity,
                          Vector2 pos) {
    WE_C(we_transform);
    ecs_set(world, entity, we_transform, {.position = pos});
}
void we_transform_add(ecs_world_t *world, ecs_entity_t entity) {
    WE_C(we_transform);
    ecs_add(world, entity, we_transform);
}

/*

Collidable

*/

void we_collidable_add(ecs_world_t *world, ecs_id_t entity) {
    WE_C(we_collidable);
    ecs_add(world, entity, we_collidable);
}

void we_collidable_set(ecs_world_t *world, ecs_id_t entity, int width,
                       int height) {
    WE_C(we_collidable);
    ecs_set(world, entity, we_collidable, {.width = width, .height = height});
}
