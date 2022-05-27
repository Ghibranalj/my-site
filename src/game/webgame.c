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

we_map_coll_bounds *head;
//

we_game_t webgame = {
    .title = "webgame",
    .width = 800,
    .height = 600,
    .fps = 60,
    .on_update = &webgame_update,
    .on_init = &webgame_init,
    .on_destroy = &webgame_destroy,
};

int main() {
    we_create_and_start(&webgame);
}

void webgame_init() {
    ecs_world_t *world = we_get_world();
    printf("world initialized\n");

    tmx_map *map = tmx_load(WE_MAP_LOCATION "0001_Level_0.tmx");
    WE_C(we_map);
    ecs_entity_t mape = ecs_new_id(world);
    ecs_add(world, mape, we_map);
    ecs_set(world, mape, we_map, {.map = map});

    player_init(world);

    head = we_get_collision_bounds(map);

    WE_C(we_coll_map);
    ecs_add(world, mape, we_coll_map);
    ecs_set(world, mape, we_coll_map, {.head = head});
}

void webgame_update(float delta) {
    we_draw_collision_bounds(head);

    // DrawRectangleLinesEx((Rectangle){.x = 0, .y = 0, .width = 16, .height =
    // 16},
    //                      2, RED);
}

void webgame_destroy() {}
