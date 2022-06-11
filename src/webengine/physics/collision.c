#include <raylib.h>
#include <raymath.h>

#include <flecs.h>

#include "../engine.h"
#include "../include/webengine.h"
#include "collisionmath.h"

#include <stdio.h>

static ecs_query_t *coll_map_filter = NULL;

Rectangle move_rec(Rectangle rec, Vector2 move) {
    rec.x += move.x;
    rec.y += move.y;
    return rec;
}

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

    WE_C(we_movable);

    we_map_coll_bounds *head = bounds;
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {

        ecs_entity_t e = it->entities[i];

        we_collidable col = colls[i];
        Vector2 pos = trans[i].position;
        Vector2 vel = movs[i].velocity;
        // vel = Vector2Scale(vel, get_last_frame_time());

        Vector2 np = Vector2Add(pos, vel);

        Rectangle bound = {
            .x = np.x,
            .y = np.y,
            .width = col.width,
            .height = col.height,
        };

        bound = move_rec(bound, vel);

        Vector2 result = Vector2Zero();

        bool collided = false;

        while (head) {
            Rectangle map = head->bound;

            Vector2 cp, cn;
            float t;
            if (DynamicRectVsRect(bound, vel, map, &cp, &cn, &t)) {
                collided = true;
                DrawRectangleLinesEx(bound, 1, BROWN);
                DrawCircle(cp.x, cp.y, 1, GREEN);

                float d = 1 - t;

                Vector2 v = (Vector2){
                    fabs(vel.x) * d,
                    fabs(vel.y) * d,
                };

                printf("v1 : %f %f\n", v.x, v.y);
                Vector2 v2 = (Vector2){
                    .x = v.x * cn.x,
                    .y = v.y * cn.y,
                };

                printf("v2 : %f %f\n", v2.x, v2.y);

                // v2 = Vector2Scale(v2, get_last_frame_time());

                printf("vel-before : %f %f\n", vel.x, vel.y);

                vel = Vector2Add(vel, v2);

                printf("vel : %f %f\n", vel.x, vel.y);
            }

            if (collided) {
                we_movable_set_vel(world, e, vel);
                printf("done\n");
            }

            head = head->next;
        }
    }
}
