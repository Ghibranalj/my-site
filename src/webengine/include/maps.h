

#pragma once
#include <raylib.h>
#include <tmx.h>

// automatically prefixed
#define WE_MAP_LOCATION "resources/tmx/tiled/"

typedef struct _we_coll_bounds {
    Rectangle bound;
    struct _we_coll_bounds *next;
} we_map_coll_bounds;

typedef struct {
    we_map_coll_bounds *head;
} we_coll_map;

we_map_coll_bounds *we_get_collision_bounds(tmx_map *map);
void we_draw_collision_bounds(we_map_coll_bounds *head);