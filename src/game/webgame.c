#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include <webengine.h>

// local function declarations
void webgame_init();
void webgame_update(float);
void webgame_destroy(void);

void player_update(float time, ecs_entity_t entity, ecs_world_t *world);

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

ecs_entity_t e2;

void webgame_init() {
    world = we_get_world();
    printf("world initialized\n");

    tmx_map *map = tmx_load(WE_MAP_LOCATION "0001_Level_0.tmx");
    WE_C(we_map);
    ecs_entity_t e = ecs_new_id(world);
    ecs_add(world, e, we_map);
    ecs_set(world, e, we_map, {.map = map});

    WE_C(we_transform);
    WE_C(we_spritesheet);
    WE_C(we_animation);

    e2 = ecs_new_id(world);
    ecs_add(world, e2, we_transform);
    ecs_set(world, e2, we_transform, {.position = {100, 100}});

    ecs_add(world, e2, we_spritesheet);
    ecs_set(world, e2, we_spritesheet,
            {.texture = LoadTexture("resources/woman.png"),
             .offset = 1,
             .width = 16,
             .height = 21});

    ecs_add(world, e2, we_animation);
    ecs_set(world, e2, we_animation,
            {.frames = we_anim_frames(2, (int[]){5, 6}),
             .num_frames = 2,
             .disabled = false,
             .speed = 5});

    WE_C(we_script);
    ecs_add(world, e2, we_script);
    ecs_set(world, e2, we_script, {.on_update = &player_update});
}

void webgame_update(float delta) {}

void webgame_destroy() {}

void player_update(float time, ecs_entity_t entity, ecs_world_t *world) {

    if (IsKeyDown(KEY_SPACE)) {
        WE_C(we_oneway_anim);

        if (ecs_has(world, entity, we_oneway_anim)) {
            ecs_remove(world, entity, we_oneway_anim);
        }
        ecs_add(world, entity, we_oneway_anim);
        ecs_set(world, entity, we_oneway_anim,
                // TODO leaky
                {.frames = we_anim_frames(3, (int[]){7, 8, 7}),
                 .num_frames = 3,
                 .speed = 5});
    }
}
