#include "graphics.h"
#include "webengine.h"

#include <stdio.h>
#include <stdlib.h>

void player_update(float time, ecs_entity_t entity, ecs_world_t *world);

void player_init(ecs_world_t *world) {
    WE_C(we_spritesheet);
    WE_C(we_animation);

    ecs_id_t player = ecs_new_id(world);

    we_transform_add(world, player);
    we_movable_add(world, player);

    we_transform_set_pos(world, player, (Vector2){100, 100});
    we_movable_set_vel(world, player, (Vector2){0, 0});

    we_spritesheet_add(world, player);
    we_spitesheet_set(world, player, "resources/woman.png", 16, 21, 1);

    ecs_add(world, player, we_animation);
    ecs_set(world, player, we_animation,
            {.speed = 5,
             .frames = we_anim_frames(2, (int[]){5, 6}),
             .num_frames = 2});

    WE_C(we_script);
    ecs_add(world, player, we_script);
    ecs_set(world, player, we_script, {.on_update = &player_update});

    WE_C(we_anim_manager);

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
    WE_C(we_collidable);

    printf("im gonna error\n");
    ecs_add(world, player, we_collidable);
    printf("hello world\n");
    ecs_set(world, player, we_collidable, {.width = 16, .height = 21});
    printf("hello world2\n");
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

    if (IsKeyDown(KEY_C)) {
        we_change_anim_mngr_index(world, entity, 1);
    }

    we_movable_set_vel(world, entity, we_get_axis(1));

    WE_C(we_transform);

    const we_transform *t = ecs_get(world, entity, we_transform);
    we_lerp_camera(t->position.x, t->position.y, 10);
    we_zoom_camera(5);
}
