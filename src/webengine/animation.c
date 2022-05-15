#include <flecs.h>
#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>

#include "include/webengine.h"

#define ANIMATION_SPEED 0.25

void we_animate_system(ecs_iter_t *it) {

    we_animation *animation = ecs_term(it, we_animation, 1);
    we_spritesheet *spitesheet = ecs_term(it, we_spritesheet, 2);

    float delta = GetFrameTime();
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {

        we_animation an = animation[i];
        ecs_id_t entity = it->entities[i];

        int *frames = an.frames;
        float time_since_last_frame = an.time_since_last_frame;
        time_since_last_frame += delta;
        if (time_since_last_frame >= ANIMATION_SPEED) {
            an.index++;
            time_since_last_frame = 0;
            if (an.index > an.num_frames) {
                an.index = 0;
            }
        }

        int offset = frames[an.index];

        we_spritesheet sp = spitesheet[i];

        WE_C(we_spritesheet);
        WE_C(we_animation);

        ecs_set(world, entity, we_spritesheet,
                {.offset = offset,
                 .texture = sp.texture,
                 .width = sp.width,
                 .height = sp.height});
        ecs_set(world, entity, we_animation,
                {.frames = frames,
                 .num_frames = an.num_frames,
                 .index = an.index,
                 .time_since_last_frame = time_since_last_frame});
    }
}

int *we_anim_frames(int num, int *frames_arr) {
    int *frames = malloc(sizeof(int) * num);
    memcpy(frames, frames_arr, sizeof(int) * num);
    return frames;
}
