#include <flecs.h>
#include <raylib.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/webengine.h"

#define DEFAULT_ANIMATION_SPEED 0.5

void we_animate_system(ecs_iter_t *it) {

    we_animation *animation = ecs_term(it, we_animation, 1);
    we_spritesheet *spitesheet = ecs_term(it, we_spritesheet, 2);

    float delta = GetFrameTime();
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {

        we_animation an = animation[i];

        if (an.disabled)
            continue;

        ecs_id_t entity = it->entities[i];

        int *frames = an.frames;
        float time_since_last_frame = an.time_since_last_frame;
        time_since_last_frame += delta;

        float speed = 1 / an.speed;
        if (speed <= 0)
            speed = DEFAULT_ANIMATION_SPEED;

        if (time_since_last_frame >= speed) {
            an.index++;
            time_since_last_frame = 0;
            if (an.index >= an.num_frames) {
                an.index = 0;
            }
        }
        WE_C(we_animation);

        ecs_set(world, entity, we_animation,
                {.frames = frames,
                 .num_frames = an.num_frames,
                 .index = an.index,
                 .time_since_last_frame = time_since_last_frame,
                 .speed = an.speed});

        int offset = frames[an.index];

        we_spritesheet sp = spitesheet[i];

        if (offset == sp.offset)
            continue;

        WE_C(we_spritesheet);

        ecs_set(world, entity, we_spritesheet,
                {.offset = offset,
                 .texture = sp.texture,
                 .width = sp.width,
                 .height = sp.height});
    }
}

void we_oneway_anim_system(ecs_iter_t *it) {
    we_oneway_anim *one_way = ecs_term(it, we_oneway_anim, 1);
    we_spritesheet *spitesheet = ecs_term(it, we_spritesheet, 2);

    float delta = GetFrameTime();
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {
        ecs_id_t entity = it->entities[i];

        we_oneway_anim an = one_way[i];

        if (an.disabled)
            continue;

        WE_C(we_animation);
        bool hasAnim = ecs_has(world, entity, we_animation);
        if (hasAnim) {

            const we_animation *ani = ecs_get(world, entity, we_animation);

            ecs_set(world, entity, we_animation,
                    {.frames = ani->frames,
                     .num_frames = ani->num_frames,
                     .index = ani->index,
                     .time_since_last_frame = 0,
                     .disabled = true,
                     .speed = ani->speed});
        }

        int *frames = an.frames;
        float time_since_last_frame = an.time_since_last_frame;
        time_since_last_frame += delta;

        bool disabled = false;

        float speed = 1 / an.speed;
        if (speed <= 0)
            speed = DEFAULT_ANIMATION_SPEED;

        if (time_since_last_frame >= speed) {
            an.index++;
            time_since_last_frame = 0;
            if (an.index >= an.num_frames) {
                an.index = 0;
                disabled = true;
            }
        }
        WE_C(we_oneway_anim);

        if (disabled && hasAnim) {
            const we_animation *ani = ecs_get(world, entity, we_animation);

            ecs_set(world, entity, we_animation,
                    {.frames = ani->frames,
                     .num_frames = ani->num_frames,
                     .index = ani->index,
                     .time_since_last_frame = 0,
                     .disabled = false,
                     .speed = ani->speed});
        }

        if (disabled) {
            ecs_remove(world, entity, we_oneway_anim);
            continue;
        }

        ecs_set(world, entity, we_oneway_anim,
                {.frames = frames,
                 .num_frames = an.num_frames,
                 .index = an.index,
                 .disabled = disabled,
                 .time_since_last_frame = time_since_last_frame,
                 .speed = an.speed});

        int offset = frames[an.index];

        we_spritesheet sp = spitesheet[i];

        if (offset == sp.offset)
            continue;

        WE_C(we_spritesheet);

        ecs_set(world, entity, we_spritesheet,
                {.offset = offset,
                 .texture = sp.texture,
                 .width = sp.width,
                 .height = sp.height});
    }
}

void we_on_delete_anim(ecs_iter_t *it) {
    we_animation *animation = ecs_term(it, we_animation, 1);

    for (int i = 0; i < it->count; i++) {
        we_animation anim = animation[i];
        if (anim.frames != NULL)
            free(anim.frames);
    }
}

void we_on_delete_oneway_anim(ecs_iter_t *it) {
    we_oneway_anim *animation = ecs_term(it, we_oneway_anim, 1);
    for (int i = 0; i < it->count; i++) {
        we_animation anim = animation[i];
        if (anim.frames != NULL)
            free(anim.frames);
    }
}

int *we_anim_frames(int num, int *frames_arr) {
    int *frames = malloc(sizeof(int) * num);
    memcpy(frames, frames_arr, sizeof(int) * num);
    return frames;
}
