#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

#include "../include/webengine.h"

void draw_system(ecs_iter_t *it) {
    sprite *sp = ecs_term(it, sprite, 1);
    transform *trans = ecs_term(it, transform, 2);

    for (int i = 0; i < it->count; i++) {
        transform tran = trans[i];

        Vector2 p = tran.position;

        sprite s = sp[i];
        Texture2D tex = s.texture;
        DrawTexture(tex, p.x, p.y, WHITE);
    }
}
void on_delete_sprite(ecs_iter_t *it) {
    sprite *sp = ecs_term(it, sprite, 1);
    for (int i = 0; i < it->count; i++) {
        sprite s = sp[i];
        Texture2D tex = s.texture;
        UnloadTexture(tex);
    }
}

void draw_spritesheet(ecs_iter_t *it) {

    spritesheet *sp = ecs_term(it, spritesheet, 1);
    transform *trans = ecs_term(it, transform, 2);
    for (int i = 0; i < it->count; i++) {

        spritesheet s = sp[i];
        Texture2D tex = s.texture;

        transform tr = trans[i];

        int width = s.width;
        int height = s.height;
        int off = (s.offset) % 9;

        int x_num = ((tex.width + 1) / s.width);
        int y_off = (off / x_num);
        int x_off = (off % x_num);

        Rectangle source_rect = {width * x_off, height * y_off, width, height};

        DrawTextureRec(tex, source_rect, tr.position, WHITE);
    }
}

void on_delete_spritesheet(ecs_iter_t *it) {
    spritesheet *sp = ecs_term(it, spritesheet, 1);
    for (int i = 0; i < it->count; i++) {
        spritesheet s = sp[i];
        UnloadTexture(s.texture);
    }
}

void spritesheet_add(ecs_world_t *world, ecs_entity_t entity) {
    C(spritesheet);
    ecs_add(world, entity, spritesheet);
}

void spitesheet_set(ecs_world_t *world, ecs_entity_t entity,
                    char *path_to_texture, int width, int height, int offset) {
    C(spritesheet);
    ecs_set(world, entity, spritesheet,
            {
                .texture = LoadTexture(path_to_texture),
                .width = width,
                .height = height,
                .offset = offset,
            });
}
