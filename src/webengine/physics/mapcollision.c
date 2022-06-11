#include "../include/logging/logging.h"
#include <stdlib.h>
#include <strings.h>

#include <flecs.h>
#include <raylib.h>
#include <tmx.h>

#include "../engine.h"
#include "../include/webengine.h"

tmx_layer *get_layer(tmx_map *map, const char *name) {
    tmx_layer *layer = map->ly_head;
    while (layer) {
        if (strcasecmp(layer->name, name) == 0) {
            return layer;
        }
        layer = layer->next;
    }
    return NULL;
}

map_coll_bounds *get_collision_bounds(tmx_map *map) {

    tmx_layer *layer = get_layer(map, COLLISION_LAYER_NAME);

    if (!layer) {
        LOG("could not find collision layer\n");
        return NULL;
    }

    // create linked list
    map_coll_bounds *head = NULL;
    map_coll_bounds *first_head = NULL;
    int count = 0;
    for (int i = 0; i < map->width; i++) {
        for (int j = 0; j < map->height; j++) {
            /* code */
            unsigned int gid = (layer->content.gids[(i * map->width) + j]) &
                               TMX_FLIP_BITS_REMOVAL;
            if (gid > map->tilecount || map->tiles[gid] == NULL) {
                continue;
            }
            tmx_tileset *ts = map->tiles[gid]->tileset;
            unsigned int w, h;

            w = ts->tile_width;
            h = ts->tile_height;

            map_coll_bounds *new_node = malloc(sizeof(map_coll_bounds));

            new_node->bound = (Rectangle){
                .x = j * w,
                .y = i * h,
                .width = w,
                .height = h,
            };

            new_node->next = NULL;

            if (head == NULL) {
                head = new_node;
                first_head = head;
                continue;
            }

            head->next = new_node;
            head = new_node;
        }
    }

    return first_head;
}

void draw_collision_bounds(map_coll_bounds *head) {
    map_coll_bounds *node = head;

    while (node) {
        DrawRectangleLinesEx(node->bound, 1, RED);
        node = node->next;
    }
}