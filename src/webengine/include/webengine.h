#pragma once
#include <ferox.h>
#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <tmx.h>

typedef void (*we_update_callback)(float time);
typedef void (*we_callback)();

typedef void (*we_script_callback)(float time, ecs_entity_t entity,
                                   ecs_world_t *t_world);
// typedef void (*we_init_callback)(ecs_world_t *world);

typedef struct {
    // CALLBACKS
    we_update_callback on_update;
    we_callback on_init;
    we_callback on_destroy;

    // ecs
    ecs_world_t *world;

    // DATA
    int width, height, fps;
    char *title;
} we_game_t;

void we_create_and_start(we_game_t *game);

ecs_world_t *we_get_world();

// engine provided components

typedef struct {
    Texture2D texture;
} we_sprite;

typedef struct {
    Texture2D texture;
    int width, height;
    int offset;
} we_spritesheet;

typedef struct {
    Vector2 position;
} we_transform;

// automatically prefixed
#define WE_MAP_LOCATION "resources/tmx/tiled/"

typedef struct {
    tmx_map *map;
} we_map;

typedef struct {
    int *frames;
    int num_frames;
    float speed;
    bool disabled;

    // internal use
    int index;
    float time_since_last_frame;
} we_animation, we_oneway_anim;

typedef struct {
    int **animations;
    int *length_of_animations;
    int len;
    int index;
} we_anim_manager;

int **we_animations(int len);

int *we_anim_frames(int num, int *frames_arr);

void we_change_anim_mngr_index(ecs_world_t *world, ecs_id_t entity, int index);

typedef struct {
    we_script_callback on_update;
} we_script;

Camera2D *we_get_camera();
void we_center_camera(float x, float y);
void we_zoom_camera(float zoom);
void we_lerp_camera(float x, float y, float speed);

Vector2 we_get_axis();

typedef struct _we_coll_bounds {
    frBody *body;
    struct _we_coll_bounds *next;
} we_map_coll_bounds;

typedef struct {
    we_map_coll_bounds *head;
} we_coll_map;

we_map_coll_bounds *we_get_collision_bounds(tmx_map *map);
void we_draw_collision_bounds(we_map_coll_bounds *head);

// Physics
typedef struct {
    frBody *body;
} we_physics_body;

#define WE_C(c) ECS_COMPONENT(we_get_world(), c)