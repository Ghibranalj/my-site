#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include <webengine.h>

// local function declarations
void webgame_init();
void webgame_update(float);
void webgame_destroy(void);

//

ecs_world_t *world = NULL;

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
    webgame.world = world;
    we_create_and_start(&webgame);
}

void webgame_init() {
    world = we_get_world();
    printf("world initialized\n");
    // create a map
    tmx_map *map = tmx_load(WE_MAP_LOCATION "0001_Level_0.tmx");
    WE_C(we_map);
    ecs_entity_t e = ecs_new_id(world);
    ecs_add(world, e, we_map);
    ecs_set(world, e, we_map, {.map = map});

    WE_C(we_transform);
    WE_C(we_spritesheet);
    WE_C(we_animation);

    ecs_entity_t e2 = ecs_new_id(world);
    ecs_add(world, e2, we_transform);
    ecs_set(world, e2, we_transform, {.position = {100, 100}});

    ecs_add(world, e2, we_spritesheet);
    ecs_set(world, e2, we_spritesheet,
            {.texture = LoadTexture("resources/woman.png"),
             .offset = 1,
             .width = 16,
             .height = 21});

    int *frames = we_anim_frames(3, (int[]){1, 2, 3});

    ecs_add(world, e2, we_animation);
    ecs_set(world, e2, we_animation,
            {.frames = frames,
             .num_frames = 3,
             .index = 0,
             .time_since_last_frame = 0});
}

void webgame_update(float delta) {
    // printf("%f\n", delta);
}

void webgame_destroy() {}
