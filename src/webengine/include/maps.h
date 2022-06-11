

#pragma once
#include <raylib.h>
#include <tmx.h>

// automatically prefixed
#define MAP_LOCATION "resources/tmx/tiled/"

typedef struct _coll_bounds {
    Rectangle bound;
    struct _coll_bounds *next;
} map_coll_bounds;

typedef struct {
    map_coll_bounds *head;
} coll_map;

map_coll_bounds *get_collision_bounds(tmx_map *map);
void draw_collision_bounds(map_coll_bounds *head);