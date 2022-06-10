
#include <stdio.h>

#include <flecs.h>
#include <raylib.h>

#include "../include/webengine.h"

void we_movable_system(ecs_iter_t *it) {
    we_movable *movs = ecs_term(it, we_movable, 1);
    we_transform *trans = ecs_term(it, we_transform, 2);

    WE_C(we_transform);
    WE_C(we_movable);

    ecs_world_t *world = it->world;
    for (int i = 0; i < it->count; ++i) {

        ecs_id_t entity = it->entities[i];

        Vector2 pos = trans[i].position;
        Vector2 vel = movs[i].velocity;
        Vector2 vt = Vector2Scale(vel, GetFrameTime());

        Vector2 np = Vector2Add(pos, vt);

        ecs_set(world, entity, we_transform, {.position = np});

        // ecs_set(world, entity, we_movable, {.velocity = (Vector2){0, 0}});
    }
}
