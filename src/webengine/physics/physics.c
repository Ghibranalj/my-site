
#include <stdio.h>

#include <flecs.h>
#include <raylib.h>

#include "../include/webengine.h"

void movable_system(ecs_iter_t *it) {
    movable *movs = ecs_term(it, movable, 1);
    transform *trans = ecs_term(it, transform, 2);

    C(transform);
    C(movable);

    ecs_world_t *world = it->world;
    for (int i = 0; i < it->count; ++i) {

        ecs_id_t entity = it->entities[i];

        Vector2 pos = trans[i].position;
        Vector2 vel = movs[i].velocity;
        Vector2 vt = Vector2Scale(vel, GetFrameTime());

        Vector2 np = Vector2Add(pos, vt);

        ecs_set(world, entity, transform, {.position = np});

        // ecs_set(world, entity, movable, {.velocity = (Vector2){0, 0}});
    }
}
