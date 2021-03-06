#include <raylib.h>
#include <raymath.h>

#include <flecs.h>

#include "../include/webengine.h"

static ecs_query_t *coll_map_filter = NULL;

Rectangle move_rec(Rectangle rec, Vector2 move) {
    rec.x += move.x;
    rec.y += move.y;
    return rec;
}

void do_collision(ecs_iter_t *it, map_coll_bounds *bounds, collidable *colls,
                  movable *movs, transform *trans);

void init_collision() {

    ecs_world_t *world = get_world();

    C(coll_map);
    coll_map_filter = ecs_query_init(
        world, &(ecs_query_desc_t){.filter.terms = {{ecs_id(coll_map)}}});
}

void map_coll_system(ecs_iter_t *it) {

    collidable *colls = ecs_term(it, collidable, 1);
    movable *movs = ecs_term(it, movable, 2);
    transform *trans = ecs_term(it, transform, 3);

    ecs_world_t *world = it->world;

    ecs_iter_t it_coll_map = ecs_query_iter(world, coll_map_filter);

    while (ecs_query_next(&it_coll_map)) {
        coll_map *map = ecs_term(&it_coll_map, coll_map, 1);

        do_collision(it, map->head, colls, movs, trans);
    }
}

void do_collision(ecs_iter_t *it, map_coll_bounds *bounds, collidable *colls,
                  movable *movs, transform *trans) {

    C(movable);

    map_coll_bounds *head = bounds;
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {

        ecs_entity_t e = it->entities[i];

        collidable col = colls[i];
        Vector2 pos = trans[i].position;
        Vector2 vel = movs[i].velocity;

        Vector2 np = Vector2Add(pos, Vector2Scale(vel, GetFrameTime()));

        Rectangle bound = {
            .x = np.x,
            .y = np.y,
            .width = col.width,
            .height = col.height,
        };

        bool collided = false;
        Vector2 result = Vector2Zero();

        while (head) {
            Rectangle map = head->bound;
            if (!CheckCollisionRecs(map, bound)) {
                head = head->next;
                continue;
            }

            collided = true;

            Rectangle col_rec = GetCollisionRec(map, bound);

            Vector2 mid_col_rec = {
                .x = col_rec.x + col_rec.width / 2,
                .y = col_rec.y + col_rec.height / 2,
            };
            Vector2 mid_bound = {
                .x = bound.x + bound.width / 2,
                .y = bound.y + bound.height / 2,
            };
            Vector2 dist = Vector2Subtract(mid_bound, mid_col_rec);

            Vector2 v = (Vector2){
                .x = dist.x / (bound.width / 2),
                .y = dist.y / (bound.height / 2),
            };

            result = Vector2Add(result, v);
            bound = move_rec(bound, v);
            head = head->next;
        }
        if (collided) {
            ecs_set(world, e, movable, {.velocity = Vector2Scale(result, 2.2)});
        }
    }
}
