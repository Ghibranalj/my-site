#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include <webengine.h>

#include "game.h"

// local function declarations
void webgame_init();
void webgame_update(float);
void webgame_destroy(void);

void player_update(float time, ecs_entity_t entity, ecs_world_t *world);

map_coll_bounds *head;
//

game_t webgame = {
    .title = "webgame",
    .width = 800,
    .height = 600,
    .fps = 120,
    .on_update = &webgame_update,
    .on_init = &webgame_init,
    .on_destroy = &webgame_destroy,
};

int main() {
    create_and_start(&webgame);
}

void webgame_init() {
    ecs_world_t *world = get_world();
    printf("world initialized\n");

    tmx_map *m = tmx_load(MAP_LOCATION "0001_Level_0.tmx");
    C(map);
    ecs_entity_t mape = ecs_new_id(world);
    ecs_add(world, mape, map);
    ecs_set(world, mape, map, {.map = m});

    player_init(world);

    head = get_collision_bounds(m);

    C(coll_map);
    ecs_add(world, mape, coll_map);
    ecs_set(world, mape, coll_map, {.head = head});
    printf("%p\n", head);
}

void webgame_update(float delta) {
    draw_collision_bounds(head);
}

void webgame_destroy() {}
