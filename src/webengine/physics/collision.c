#include <raylib.h>
#include <raymath.h>

#include <flecs.h>

#include "../include/webengine.h"

static ecs_query_t *coll_map_filter = NULL;

void init_collisions() {

    ecs_world_t *world = we_get_world();

    WE_C(we_coll_map);
    coll_map_filter = ecs_query_init(
        world, &(ecs_query_desc_t){.filter.terms = {{ecs_id(we_coll_map)}}});
}

void we_map_coll_system(ecs_iter_t *it) {

    we_collidable *colls = ecs_term(it, we_collidable, 1);

    ecs_world_t *world = it->world;

    ecs_iter_t it_coll_map = ecs_query_iter(world, coll_map_filter);
}
