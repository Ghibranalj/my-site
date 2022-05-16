#include "webengine.h"
#include <flecs.h>
#include <raylib.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int curr_anim;
} player_c;

void player_update(float time, ecs_entity_t entity, ecs_world_t *world);

void player_init(ecs_world_t *world) {
    WE_C(we_transform);
    WE_C(we_spritesheet);
    WE_C(we_animation);

    ecs_id_t player = ecs_new_id(world);
    ecs_add(world, player, we_transform);
    ecs_set(world, player, we_transform, {.position = {100, 100}});

    ecs_add(world, player, we_spritesheet);
    ecs_set(world, player, we_spritesheet,
            {.texture = LoadTexture("resources/woman.png"),
             .offset = 1,
             .width = 16,
             .height = 21});

    ecs_add(world, player, we_animation);
    ecs_set(world, player, we_animation, {.speed = 5});

    WE_C(we_script);
    ecs_add(world, player, we_script);
    ecs_set(world, player, we_script, {.on_update = &player_update});

    WE_C(we_anim_manager);
    ecs_add(world, player, we_anim_manager);

    int **animations = we_animations(2);
    animations[0] = we_anim_frames(2, (int[]){5, 6});
    animations[1] = we_anim_frames(2, (int[]){7, 8});
    int *length_of_animations = we_anim_frames(2, (int[]){2, 2});

    ecs_set(world, player, we_anim_manager,
            {
                .animations = animations,
                .length_of_animations = length_of_animations,
                .len = 2,
                .index = 0,
            });

    WE_C(player_c);
    ecs_add(world, player, player_c);
    ecs_set(world, player, player_c, {.curr_anim = 0});
}

void player_update(float time, ecs_entity_t entity, ecs_world_t *world) {

    WE_C(player_c);
    const player_c *player = ecs_get(world, entity, player_c);

    if (IsKeyDown(KEY_SPACE)) {
        WE_C(we_oneway_anim);

        if (ecs_has(world, entity, we_oneway_anim)) {
            ecs_remove(world, entity, we_oneway_anim);
        }
        ecs_add(world, entity, we_oneway_anim);
        ecs_set(world, entity, we_oneway_anim,
                {.frames = we_anim_frames(3, (int[]){7, 8, 7}),
                 .num_frames = 3,
                 .speed = 7});
    }

    if (IsKeyReleased(KEY_C)) {
        int index = (player->curr_anim + 1) % 2;
        we_change_anim_mngr_index(world, entity, index);
        ecs_set(world, entity, player_c, {.curr_anim = index});
    }
}