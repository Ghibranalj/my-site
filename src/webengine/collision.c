#include <flecs.h>
#include <stdio.h>

#include "engine.h"
#include "include/webengine.h"

ecs_query_t *coll_map_filter;
void we_init_collision() {
    ecs_world_t *world = we_get_world();

    WE_C(we_coll_map);
    coll_map_filter = ecs_query_init(
        world, &(ecs_query_desc_t){.filter.terms = {{ecs_id(we_coll_map)}}});
}

void we_collision_system(ecs_iter_t *it) {
    we_transform *trans = ecs_term(it, we_transform, 1);
    we_coll_bound *bounds = ecs_term(it, we_coll_bound, 2);
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {
        we_transform tr = trans[i];
        we_coll_bound b = bounds[i];
        ecs_id_t entity = it->entities[i];

        Rectangle rec_bound = {.x = tr.position.x,
                               .y = tr.position.y,
                               .width = b.width,
                               .height = b.height};

        // TODO this is O(n^2)
        ecs_iter_t it2 = ecs_query_iter(world, coll_map_filter);
        while (ecs_query_next(&it2)) {
            we_coll_map *map = ecs_term(&it2, we_coll_map, 1);

            we_map_coll_bounds *head = map->head;
            while (head) {
                Rectangle rec_map = head->rec;

                if (!CheckCollisionRecs(rec_bound, rec_map)) {
                    head = head->next;
                    continue;
                }

                Rectangle rec_clip = GetCollisionRec(rec_bound, rec_map);
                DrawRectangleLinesEx(rec_clip, 2, RED);
                int dx = rec_clip.width;

                WE_C(we_transform);

                head = head->next;
            }
        }
    }
}