#include "../include/webengine.h"

/*

Movable

*/

void movable_set_vel(ecs_world_t *world, ecs_entity_t entity, Vector2 vel) {
    C(movable);

    ecs_set(world, entity, movable, {.velocity = vel});
}

void movable_add(ecs_world_t *world, ecs_entity_t entity) {
    C(movable);
    ecs_add(world, entity, movable);
}

/*

Transform

*/

void transform_set_pos(ecs_world_t *world, ecs_entity_t entity, Vector2 pos) {
    C(transform);
    ecs_set(world, entity, transform, {.position = pos});
}
void transform_add(ecs_world_t *world, ecs_entity_t entity) {
    C(transform);
    ecs_add(world, entity, transform);
}

/*

Collidable

*/

void collidable_add(ecs_world_t *world, ecs_id_t entity) {
    C(collidable);
    ecs_add(world, entity, collidable);
}

void collidable_set(ecs_world_t *world, ecs_id_t entity, int width,
                    int height) {
    C(collidable);
    ecs_set(world, entity, collidable, {.width = width, .height = height});
}