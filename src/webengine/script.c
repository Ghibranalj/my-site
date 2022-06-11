#include "include/webengine.h"

#include <flecs.h>
#include <raylib.h>

void script_system(ecs_iter_t *it) {

    script *s = ecs_term(it, script, 1);
    ecs_world_t *world = it->world;
    float delta = GetFrameTime();
    for (int i = 0; i < it->count; i++) {
        ecs_id_t entity = it->entities[i];
        if (!ecs_is_alive(world, entity))
            continue;

        s[i].on_update(delta, entity, world);
    }
}
