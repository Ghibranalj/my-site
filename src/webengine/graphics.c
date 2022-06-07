#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

#include "include/webengine.h"

void we_draw_system(ecs_iter_t *it) {
    we_sprite *sprite = ecs_term(it, we_sprite, 1);
    we_transform *trans = ecs_term(it, we_transform, 2);

    for (int i = 0; i < it->count; i++) {
        we_transform tran = trans[i];

        Vector2 p = tran.position;

        we_sprite sp = sprite[i];
        Texture2D tex = sp.texture;
        DrawTexture(tex, p.x, p.y, WHITE);
    }
}
void we_on_delete_sprite(ecs_iter_t *it) {
    we_sprite *sprite = ecs_term(it, we_sprite, 1);
    for (int i = 0; i < it->count; i++) {
        we_sprite sp = sprite[i];
        Texture2D tex = sp.texture;
        UnloadTexture(tex);
    }
}

void we_draw_spritesheet(ecs_iter_t *it) {

    we_spritesheet *spritesheet = ecs_term(it, we_spritesheet, 1);
    we_transform *trans = ecs_term(it, we_transform, 2);
    for (int i = 0; i < it->count; i++) {

        we_spritesheet sp = spritesheet[i];
        Texture2D tex = sp.texture;

        we_transform tr = trans[i];

        int width = sp.width;
        int height = sp.height;
        int off = (sp.offset) % 9;

        int x_num = ((tex.width + 1) / sp.width);
        int y_off = (off / x_num);
        int x_off = (off % x_num);

        Rectangle source_rect = {width * x_off, height * y_off, width, height};

        DrawTextureRec(tex, source_rect, tr.position, WHITE);
    }
}

void we_on_delete_spritesheet(ecs_iter_t *it) {
    we_spritesheet *spritesheet = ecs_term(it, we_spritesheet, 1);
    for (int i = 0; i < it->count; i++) {
        we_spritesheet sp = spritesheet[i];
        UnloadTexture(sp.texture);
    }
}

void we_spritesheet_add(ecs_world_t *world, ecs_entity_t entity) {
    WE_C(we_spritesheet);
    ecs_add(world, entity, we_spritesheet);
}

void we_spitesheet_set(ecs_world_t *world, ecs_entity_t entity,
                       char *path_to_texture, int width, int height,
                       int offset) {
    WE_C(we_spritesheet);
    ecs_set(world, entity, we_spritesheet,
            {
                .texture = LoadTexture(path_to_texture),
                .width = width,
                .height = height,
                .offset = offset,
            });
}
