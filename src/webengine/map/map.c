#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <raylib.h>
#include <tmx.h>

#include "../engine.h"
#include "../include/webengine.h"

void *raylib_tex_loader(const char *path) {
    Texture2D *returnValue = malloc(sizeof(Texture2D));
    *returnValue = LoadTexture(path);
    return returnValue;
}

void raylib_free_tex(void *ptr) {
    UnloadTexture(*((Texture2D *)ptr));
    free(ptr);
}

Color int_to_color(int color) {
    tmx_col_bytes res = tmx_col_to_bytes(color);
    return *((Color *)&res);
}
void draw_all_layers(tmx_map *map, tmx_layer *layers);

void draw_image_layer(tmx_image *image) {
    Texture2D *texture = (Texture2D *)image->resource_image;
    DrawTexture(*texture, 0, 0, WHITE);
}

#define LINE_THICKNESS 2.5

void draw_polyline(double offset_x, double offset_y, double **points,
                   int points_count, Color color) {
    int i;
    for (i = 1; i < points_count; i++) {
        DrawLineEx(
            (Vector2){offset_x + points[i - 1][0], offset_y + points[i - 1][1]},
            (Vector2){offset_x + points[i][0], offset_y + points[i][1]},
            LINE_THICKNESS, color);
    }
}

void draw_polygon(double offset_x, double offset_y, double **points,
                  int points_count, Color color) {
    draw_polyline(offset_x, offset_y, points, points_count, color);
    if (points_count > 2) {
        DrawLineEx((Vector2){offset_x + points[0][0], offset_y + points[0][1]},
                   (Vector2){offset_x + points[points_count - 1][0],
                             offset_y + points[points_count - 1][1]},
                   LINE_THICKNESS, color);
    }
}

void draw_objects(tmx_object_group *objgr) {
    tmx_object *head = objgr->head;
    Color color = int_to_color(objgr->color);

    while (head) {
        if (head->visible) {

            if (head->obj_type == OT_SQUARE) {
                DrawRectangleLinesEx(
                    (Rectangle){head->x, head->y, head->width, head->height},
                    LINE_THICKNESS, color);
            } else if (head->obj_type == OT_POLYGON) {
                draw_polygon(head->x, head->y, head->content.shape->points,
                             head->content.shape->points_len, color);
            } else if (head->obj_type == OT_POLYLINE) {
                draw_polyline(head->x, head->y, head->content.shape->points,
                              head->content.shape->points_len, color);
            } else if (head->obj_type == OT_ELLIPSE) {
                DrawEllipseLines(head->x + head->width / 2.0,
                                 head->y + head->height / 2.0,
                                 head->width / 2.0, head->height / 2.0, color);
            }
        }
        head = head->next;
    }
}

void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw,
               unsigned int sh, unsigned int dx, unsigned int dy, float opacity,
               unsigned int flags) {

    Texture2D texture = *((Texture2D *)image);
    // DrawTexture(texture, 0, 0, WHITE);
    float norm_opacity = opacity * 255;
    char color = (char)norm_opacity;
    DrawTextureRec(texture, (Rectangle){sx, sy, sw, sh}, (Vector2){dx, dy},
                   (Color){color, color, color, color});
}

void draw_layer(tmx_map *map, tmx_layer *layer) {
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    void *image;
    op = layer->opacity;
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            gid = (layer->content.gids[(i * map->width) + j]) &
                  TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                x = map->tiles[gid]->ul_x;
                y = map->tiles[gid]->ul_y;
                w = ts->tile_width;
                h = ts->tile_height;
                if (im) {
                    image = im->resource_image;
                } else {
                    image = ts->image->resource_image;
                }
                flags = (layer->content.gids[(i * map->width) + j]) &
                        ~TMX_FLIP_BITS_REMOVAL;
                draw_tile(image, x, y, w, h, j * ts->tile_width,
                          i * ts->tile_height, op,
                          flags); // Function to be implemented
            }
        }
    }
}

void draw_all_layers(tmx_map *map, tmx_layer *layers) {
    while (layers) {

        if (!layers->visible) {
            layers = layers->next;
            continue;
        }
        if (strcasecmp(layers->name, COLLISION_LAYER_NAME) == 0) {
            layers = layers->next;
            continue;
        }
        // recursive call
        if (layers->type == L_GROUP) {
            draw_all_layers(map, layers->content.group_head);
            layers = layers->next;
            continue;
        }

        if (layers->type == L_OBJGR) {
            draw_objects(layers->content.objgr);
            layers = layers->next;
            continue;
        }

        if (layers->type == L_IMAGE) {
            draw_image_layer(layers->content.image);
            layers = layers->next;
            continue;
        }

        if (layers->type == L_LAYER) {
            draw_layer(map, layers);
            layers = layers->next;
            continue;
        }
    }
}

void we_draw_map_system(ecs_iter_t *it) {
    we_map *map_cs = ecs_term(it, we_map, 1);

    for (int i = 0; i < it->count; i++) {
        we_map map_c = map_cs[i];

        tmx_map *map = map_c.map;

        ClearBackground(int_to_color(map->backgroundcolor));
        draw_all_layers(map, map->ly_head);
    }
}

void we_on_delete_map(ecs_iter_t *it) {
    we_map *map_cs = ecs_term(it, we_map, 1);

    for (int i = 0; i < it->count; i++) {
        we_map map_c = map_cs[i];

        tmx_map *map = map_c.map;
        tmx_map_free(map);
    }
}

void we_init_map() {
    tmx_img_load_func = raylib_tex_loader;
    tmx_img_free_func = raylib_free_tex;
}