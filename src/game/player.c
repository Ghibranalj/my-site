#include "graphics.h"
#include "webengine.h"

#include <logging/logging.h>
#include <stdlib.h>

void player_update(float time, ecs_entity_t entity, ecs_world_t *world);

void player_init(ecs_world_t *world) {
    C(spritesheet);
    C(animation);

    ecs_id_t player = ecs_new_id(world);

    transform_add(world, player);
    movable_add(world, player);

    transform_set_pos(world, player, (Vector2){100, 100});
    movable_set_vel(world, player, (Vector2){0, 0});

    spritesheet_add(world, player);
    spitesheet_set(world, player, "resources/woman.png", 16, 21, 1);

    ecs_add(world, player, animation);
    ecs_set(
        world, player, animation,
        {.speed = 5, .frames = anim_frames(2, (int[]){5, 6}), .num_frames = 2});

    C(script);
    ecs_add(world, player, script);
    ecs_set(world, player, script, {.on_update = &player_update});

    C(anim_manager);

    int **anims = animations(2);

    anims[0] = anim_frames(2, (int[]){5, 6});
    anims[1] = anim_frames(2, (int[]){7, 8});
    int *length_of_animations = anim_frames(2, (int[]){2, 2});

    ecs_set(world, player, anim_manager,
            {
                .animations = anims,
                .length_of_animations = length_of_animations,
                .len = 2,
                .index = 0,
            });
    C(collidable);

    ecs_add(world, player, collidable);
    ecs_set(world, player, collidable, {.width = 16, .height = 21});

    ERROR("Player initialized\n");
}

void player_update(float time, ecs_entity_t entity, ecs_world_t *world) {

    if (IsKeyDown(KEY_SPACE)) {
        C(oneway_anim);

        if (ecs_has(world, entity, oneway_anim)) {
            ecs_remove(world, entity, oneway_anim);
        }
        ecs_add(world, entity, oneway_anim);
        ecs_set(world, entity, oneway_anim,
                {.frames = anim_frames(3, (int[]){7, 8, 7}),
                 .num_frames = 3,
                 .speed = 7});
    }

    if (IsKeyDown(KEY_C)) {
        change_anim_mngr_index(world, entity, 1);
    }

    C(transform);

    const transform *t = ecs_get(world, entity, transform);
    lerp_camera(t->position.x, t->position.y, 10);
    zoom_camera(5);

    movable_set_vel(world, entity, get_axis(100));
}
