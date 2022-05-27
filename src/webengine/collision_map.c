#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <ferox.h>
#include <flecs.h>
#include <raylib.h>
#include <tmx.h>

#include "engine.h"
#include "include/webengine.h"

#define COLL_MAP_MATERIAL ((frMaterial){0, 0, 0, 0})

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

we_map_coll_bounds *we_get_collision_bounds(tmx_map *map) {

    tmx_layer *layer = get_layer(map, COLLISION_LAYER_NAME);

    if (!layer) {
        printf("could not find collision layer\n");
        return NULL;
    }

    // create linked list
    we_map_coll_bounds *head = NULL;
    we_map_coll_bounds *first_head = NULL;
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

            Vector2 pos = {(j * 16) + (w / 2), (i * 16) + (h / 2)};

            // add to linked list
            we_map_coll_bounds *new_node = malloc(sizeof(we_map_coll_bounds));
            frBody *body = frCreateBodyFromShape(
                FR_BODY_STATIC, FR_FLAG_INFINITE_MASS,
                frVec2PixelsToMeters(pos),
                frCreateRectangle(COLL_MAP_MATERIAL, frNumberPixelsToMeters(w),
                                  frNumberPixelsToMeters(h)));
            new_node->body = body;
            new_node->next = NULL;
            if (head == NULL) {
                head = new_node;
                first_head = new_node;
                continue;
            }
            count++;

            head->next = new_node;
            head = new_node;
        }
    }

    printf("%d collision bodies\n", count);

    return first_head;
}

void we_draw_collision_bounds(we_map_coll_bounds *head) {
    while (head) {
        // Rectangle rec = head->rec;
        // DrawRectangleLinesEx(rec, 1, GREEN);
        head = head->next;
    }
}

void we_coll_map_on_set(ecs_iter_t *it) {
    we_coll_map *maps = ecs_term(it, we_coll_map, 1);

    int count = it->count;
    for (int i = 0; i < count; i++) {
        we_coll_map map = maps[i];

        we_map_coll_bounds *bounds = map.head;

        while (bounds) {
            frBody *body = bounds->body;
            we_physics_add_to_world(body);
            bounds = bounds->next;
        }
    }
}