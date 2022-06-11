#include <flecs.h>
#include <raylib.h>

#include "../include/logging/logging.h"
#include <stdbool.h>
#include <stdlib.h>

#include "../include/webengine.h"

#define DEFAULT_ANIMATION_SPEED 0.5

void animate_system(ecs_iter_t *it) {

    animation *anims = ecs_term(it, animation, 1);
    spritesheet *spitesheet = ecs_term(it, spritesheet, 2);

    float delta = GetFrameTime();
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {

        animation an = anims[i];

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
        C(animation);

        ecs_set(world, entity, animation,
                {.frames = frames,
                 .num_frames = an.num_frames,
                 .index = an.index,
                 .time_since_last_frame = time_since_last_frame,
                 .speed = an.speed});

        int offset = frames[an.index];

        spritesheet sp = spitesheet[i];

        if (offset == sp.offset)
            continue;

        C(spritesheet);

        ecs_set(world, entity, spritesheet,
                {.offset = offset,
                 .texture = sp.texture,
                 .width = sp.width,
                 .height = sp.height});
    }
}

void oneway_anim_system(ecs_iter_t *it) {
    oneway_anim *one_way = ecs_term(it, oneway_anim, 1);
    spritesheet *spitesheet = ecs_term(it, spritesheet, 2);

    float delta = GetFrameTime();
    ecs_world_t *world = it->world;

    for (int i = 0; i < it->count; i++) {
        ecs_id_t entity = it->entities[i];

        oneway_anim an = one_way[i];

        if (an.disabled)
            continue;

        C(animation);
        bool hasAnim = ecs_has(world, entity, animation);
        if (hasAnim) {

            const animation *ani = ecs_get(world, entity, animation);

            ecs_set(world, entity, animation,
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
        C(oneway_anim);

        if (disabled && hasAnim) {
            const animation *ani = ecs_get(world, entity, animation);

            ecs_set(world, entity, animation,
                    {.frames = ani->frames,
                     .num_frames = ani->num_frames,
                     .index = ani->index,
                     .time_since_last_frame = 0,
                     .disabled = false,
                     .speed = ani->speed});
        }

        if (disabled) {
            ecs_remove(world, entity, oneway_anim);
            continue;
        }

        ecs_set(world, entity, oneway_anim,
                {.frames = frames,
                 .num_frames = an.num_frames,
                 .index = an.index,
                 .disabled = disabled,
                 .time_since_last_frame = time_since_last_frame,
                 .speed = an.speed});

        int offset = frames[an.index];

        spritesheet sp = spitesheet[i];

        if (offset == sp.offset)
            continue;

        C(spritesheet);

        ecs_set(world, entity, spritesheet,
                {.offset = offset,
                 .texture = sp.texture,
                 .width = sp.width,
                 .height = sp.height});
    }
}

void on_delete_anim(ecs_iter_t *it) {
    C(anim_manager);
    animation *anims = ecs_term(it, animation, 1);

    for (int i = 0; i < it->count; i++) {
        animation anim = anims[i];

        ecs_id_t ent = it->entities[i];

        if (ecs_has(it->world, ent, anim_manager))
            continue;

        if (anim.frames == NULL)
            continue;
        free(anim.frames);
    }
}

void on_delete_oneway_anim(ecs_iter_t *it) {
    oneway_anim *anims = ecs_term(it, oneway_anim, 1);
    for (int i = 0; i < it->count; i++) {
        animation anim = anims[i];
        if (anim.frames != NULL)
            free(anim.frames);
    }
}

void on_delete_anim_manager(ecs_iter_t *it) {
    anim_manager *managers = ecs_term(it, anim_manager, 1);
    for (int i = 0; i < it->count; i++) {
        anim_manager anim = managers[i];

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

int *anim_frames(int num, int *frames_arr) {
    int *frames = malloc(sizeof(int) * num);
    memcpy(frames, frames_arr, sizeof(int) * num);
    return frames;
}

int **animations(int len) {
    int **animations = malloc(sizeof(int *) * len);
    for (int i = 0; i < len; i++) {
        animations[i] = NULL;
    }
    return animations;
}

void on_mngr_set(ecs_iter_t *it) {
    C(animation);
    anim_manager *manager = ecs_term(it, anim_manager, 1);

    for (int i = 0; i < it->count; i++) {
        anim_manager mngr = manager[i];

        ecs_id_t entity = it->entities[i];

        int len = mngr.len;
        int index = mngr.index;
        if (index >= len)
            continue;

        if (!ecs_has(it->world, entity, animation))
            continue;

        const animation *anim = ecs_get(it->world, entity, animation);

        ecs_set(it->world, entity, animation,
                {.frames = mngr.animations[index],
                 .num_frames = mngr.length_of_animations[index],
                 .index = 0,
                 .time_since_last_frame = 0,
                 .disabled = anim->disabled,
                 .speed = anim->speed});
    }
}

void change_anim_mngr_index(ecs_world_t *world, ecs_id_t entity, int index) {
    C(anim_manager);
    const anim_manager *manager = ecs_get(world, entity, anim_manager);
    ecs_set(world, entity, anim_manager,
            {
                .animations = manager->animations,
                .length_of_animations = manager->length_of_animations,
                .len = manager->len,
                .index = index,
            });
}