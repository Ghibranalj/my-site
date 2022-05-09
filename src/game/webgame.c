#include <raylib.h>
#include <stdio.h>
#include <webengine.h>

// local function declarations
void webgame_init(ecs_world_t *);
void webgame_update(float);
void webgame_destroy(void);

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
    we_create(&webgame);
}

void webgame_init(ecs_world_t *world) {
    printf("Starting webgame\n");
    ecs_id_t player;
    player = ecs_new_id(we_get_world());

    WE_RC(we_position);
    WE_RC(we_sprite);

    Texture2D texture = LoadTexture("resources/scarfy.png");
    we_position pos = {.x = 100, .y = 100};
    ecs_add(world, player, we_position);
    ecs_set(world, player, we_position, {.x = 100, .y = 100});
    ecs_add(world, player, we_sprite);
    ecs_set(world, player, we_sprite, {.texture = texture});
}

void webgame_update(float delta) {}

void webgame_destroy() {}
