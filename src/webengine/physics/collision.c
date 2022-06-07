#include <raylib.h>
#include <raymath.h>

#include <flecs.h>

#include "../include/webengine.h"

static ecs_query_t *coll_map_filter = NULL;

void do_collision(ecs_iter_t *it, we_map_coll_bounds *bounds,
                  we_collidable *colls, we_movable *movs, we_transform *trans);

void init_collision() {

    ecs_world_t *world = we_get_world();

    WE_C(we_coll_map);
    coll_map_filter = ecs_query_init(
        world, &(ecs_query_desc_t){.filter.terms = {{ecs_id(we_coll_map)}}});
}

void we_map_coll_system(ecs_iter_t *it) {

    we_collidable *colls = ecs_term(it, we_collidable, 1);
    we_movable *movs = ecs_term(it, we_movable, 2);
    we_transform *trans = ecs_term(it, we_transform, 3);

    ecs_world_t *world = it->world;

    ecs_iter_t it_coll_map = ecs_query_iter(world, coll_map_filter);

    while (ecs_query_next(&it_coll_map)) {
        we_coll_map *map = ecs_term(&it_coll_map, we_coll_map, 1);

        do_collision(it, map->head, colls, movs, trans);
    }
}

void do_collision(ecs_iter_t *it, we_map_coll_bounds *bounds,
                  we_collidable *colls, we_movable *movs, we_transform *trans) {

    we_map_coll_bounds *head = bounds;
    ecs_world_t *world = it->world;
    while (head) {

        for (int i = 0; i < it->count; i++) {

            ecs_entity_t e = it->entities[i];

            we_collidable col = colls[i];
            Vector2 tr = trans[i].position;
            Vector2 vel = movs[i].velocity;

            Rectangle map = head->bound;
            Rectangle coll = {
                .x = tr.x,
                .y = tr.y,
                .width = col.width,
                .height = col.height,
            };

            if (CheckCollisionRecs(map, coll)) {

                Rectangle col_rec = GetCollisionRec(map, coll);

                DrawRectangleRec(col_rec, GREEN);
            }
        }
        head = head->next;
    }
}
