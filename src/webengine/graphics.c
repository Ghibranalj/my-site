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

int o = 0;

void we_draw_spritesheet(ecs_iter_t *it) {

    we_spritesheet *spritesheet = ecs_term(it, we_spritesheet, 1);
    we_transform *trans = ecs_term(it, we_transform, 2);
    for (int i = 0; i < it->count; i++) {

        we_spritesheet sp = spritesheet[i];
        Texture2D tex = sp.texture;

        we_transform tr = trans[i];

        int width = sp.width;
        int height = sp.height;
        if (IsKeyPressed(KEY_SPACE))
            o++;
        int off = (sp.offset + o) % 9;
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
