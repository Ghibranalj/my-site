#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

#include "include/webengine.h"

void we_draw_system(ecs_iter_t *it) {
    we_sprite *sprite = ecs_term(it, we_sprite, 1);
    we_position *pos = ecs_term(it, we_position, 2);

    for (int i = 0; i < it->count; i++) {
        we_position p = pos[i];
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
