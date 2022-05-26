#include "webengine.h"

#include <stdio.h>

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

    WE_C(we_coll_bound);
    ecs_add(world, player, we_coll_bound);
    ecs_set(world, player, we_coll_bound, {.width = 16, .height = 21});
}

void player_update(float time, ecs_entity_t entity, ecs_world_t *world) {

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
        we_change_anim_mngr_index(world, entity, 1);
    }

    WE_C(we_transform);

    const we_transform *t = ecs_get(world, entity, we_transform);
    we_lerp_camera(t->position.x, t->position.y, 10);
    we_zoom_camera(5);

    ecs_set(world, entity, we_transform,
            {.position = Vector2Add(t->position, we_get_axis())});

    Rectangle bounds = {
        .x = t->position.x, .y = t->position.y, .width = 16, .height = 21};
    DrawRectangleLinesEx(bounds, 2, YELLOW);
}