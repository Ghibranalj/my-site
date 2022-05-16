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

        if (an.disabled || an.num_frames == 0 || an.frames == NULL)
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
    WE_C(we_anim_manager);
    we_animation *animation = ecs_term(it, we_animation, 1);

    for (int i = 0; i < it->count; i++) {
        we_animation anim = animation[i];

        ecs_id_t ent = it->entities[i];

        if (ecs_has(it->world, ent, we_anim_manager))
            continue;

        if (anim.frames == NULL)
            continue;
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

void we_on_delete_anim_manager(ecs_iter_t *it) {
    we_anim_manager *anim_manager = ecs_term(it, we_anim_manager, 1);
    for (int i = 0; i < it->count; i++) {
        we_anim_manager anim = anim_manager[i];

        int len = anim.len;
        for (int j = 0; j < len; j++) {
            if (anim.animations[j] != NULL)
                free(anim.animations[j]);
        }
        if (anim.animations != NULL)
            free(anim.animations);

        if (anim.length_of_animations != NULL)
            free(anim.length_of_animations);
    }
}

int *we_anim_frames(int num, int *frames_arr) {
    int *frames = malloc(sizeof(int) * num);
    memcpy(frames, frames_arr, sizeof(int) * num);
    return frames;
}

int **we_animations(int len) {
    int **animations = malloc(sizeof(int *) * len);
    for (int i = 0; i < len; i++) {
        animations[i] = NULL;
    }
    return animations;
}

void we_on_mngr_set(ecs_iter_t *it) {
    WE_C(we_animation);
    we_anim_manager *manager = ecs_term(it, we_anim_manager, 1);

    for (int i = 0; i < it->count; i++) {
        we_anim_manager mngr = manager[i];

        ecs_id_t entity = it->entities[i];

        int len = mngr.len;
        int index = mngr.index;
        if (index >= len)
            continue;

        if (!ecs_has(it->world, entity, we_animation))
            continue;

        const we_animation *anim = ecs_get(it->world, entity, we_animation);

        ecs_set(it->world, entity, we_animation,
                {.frames = mngr.animations[index],
                 .num_frames = mngr.length_of_animations[index],
                 .index = 0,
                 .time_since_last_frame = 0,
                 .disabled = anim->disabled,
                 .speed = anim->speed});
    }
}

void we_change_anim_mngr_index(ecs_world_t *world, ecs_id_t entity, int index) {
    WE_C(we_anim_manager);
    const we_anim_manager *anim_manager =
        ecs_get(world, entity, we_anim_manager);
    ecs_set(world, entity, we_anim_manager,
            {
                .animations = anim_manager->animations,
                .length_of_animations = anim_manager->length_of_animations,
                .len = anim_manager->len,
                .index = index,
            });
}