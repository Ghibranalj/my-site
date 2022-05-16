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

//

we_game_t webgame = {
    .title = "webgame",
    .width = 800,
    .height = 600,
    .fps = 120,
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
    ecs_entity_t e = ecs_new_id(world);
    ecs_add(world, e, we_map);
    ecs_set(world, e, we_map, {.map = map});

    player_init(world);
}

void webgame_update(float delta) {}

void webgame_destroy() {}
