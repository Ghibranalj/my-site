#include "include/webengine.h"

#include <flecs.h>
#include <raylib.h>

void we_script_system(ecs_iter_t *it) {

    we_script *script = ecs_term(it, we_script, 1);
    ecs_world_t *world = it->world;
    float delta = GetFrameTime();
    for (int i = 0; i < it->count; i++) {
        ecs_id_t entity = it->entities[i];
        if (!ecs_is_alive(world, entity))
            continue;

        script[i].on_update(delta, entity, world);
    }
}
