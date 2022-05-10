#include <raylib.h>
#include <stdio.h>
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

    printf("Starting webgame\n");
    ecs_id_t player;
    player = ecs_new_id(we_get_world());

    WE_C(we_position);
    WE_C(we_sprite);

    Texture2D texture = LoadTexture("resources/scarfy.png");
    we_position pos = {.x = 100, .y = 100};
    ecs_add(world, player, we_position);
    ecs_set(world, player, we_position, {.x = 100, .y = 100});
    ecs_add(world, player, we_sprite);
    ecs_set(world, player, we_sprite, {.texture = texture});
}

void webgame_update(float delta) {}

void webgame_destroy() {}
