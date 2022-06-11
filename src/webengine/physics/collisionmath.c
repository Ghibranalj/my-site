#include <raylib.h>
#include <raymath.h>

#include <stdio.h>

#include "../include/webengine.h"

#include "collisionmath.h"

#define swap(a, b)                                                             \
    {                                                                          \
        typeof(a) tmp = a;                                                     \
        a = b;                                                                 \
        b = tmp;                                                               \
    }
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
// #define isnan(x) ((x) != (x))

Vector2 vec2_divide_el(Vector2 v1, Vector2 v2) {
    return (Vector2){
        .x = v1.x / v2.x,
        .y = v1.y / v2.y,
    };
}

bool ray_intersects_rect(const Vector2 ray_origin, Vector2 ray_dir,
                         Rectangle rec, Vector2 *contact_point,
                         Vector2 *contact_normal, float *contact_distance) {

    *contact_normal = (Vector2){0, 0};
    *contact_point = (Vector2){0, 0};
    *contact_distance = 0;

    // TODO add invdir

    // middle of the rectangle
    Vector2 rec_pos = {
        .x = rec.x,
        .y = rec.y,
    };

    Vector2 rec_size = {
        .x = rec.width,
        .y = rec.height,
    };

    Vector2 t_near =
        vec2_divide_el(Vector2Subtract(rec_pos, ray_origin), ray_dir);

    Vector2 t_far = vec2_divide_el(
        Vector2Subtract(Vector2Add(rec_pos, rec_size), ray_origin), ray_dir);

    if (isnan(t_near.x) || isnan(t_near.y)) {
        return false;
    }
    if (isnan(t_far.x) || isnan(t_far.y)) {
        return false;
    }

    // printf("here\n");
    if (t_near.x > t_far.x) {
        swap(t_near.x, t_far.x);
    }
    if (t_near.y > t_far.y) {
        swap(t_near.y, t_far.y);
    }

    if (t_near.x > t_far.y || t_near.y > t_far.x) {
        return false;
    }
    // printf("here2\n");

    float t_hit_near = max(t_near.x, t_near.y);
    float t_hit_far = min(t_far.x, t_far.y);

    *contact_distance = t_hit_near;

    if (t_hit_far < 0) {
        return false;
    }

    *contact_point = Vector2Add(ray_origin, Vector2Scale(ray_dir, t_hit_near));

    if (t_near.x > t_near.y) {
        if (ray_dir.x < 0) {
            *contact_normal = (Vector2){.x = 1, .y = 0};
        } else {
            *contact_normal = (Vector2){.x = -1, .y = 0};
        }
    } else if (t_near.x < t_near.y) {
        if (ray_dir.y < 0) {
            *contact_normal = (Vector2){.x = 0, .y = 1};
        } else {
            *contact_normal = (Vector2){.x = 0, .y = -1};
        }
    }

    return true;
}

bool DynamicRectVsRect(Rectangle moving_rec, Vector2 vel, Rectangle static_rec,
                       Vector2 *contact_point, Vector2 *contact_normal,
                       float *contact_time) {

    if (vel.x == 0 && vel.y == 0) {
        return false;
    }

    Rectangle expanded = (Rectangle){
        .x = static_rec.x - moving_rec.width / 2,
        .y = static_rec.y - moving_rec.height / 2,
        .width = static_rec.width + moving_rec.width,
        .height = static_rec.height + moving_rec.height,
    };

    Vector2 ray_origin = (Vector2){.x = moving_rec.x + moving_rec.width / 2,
                                   .y = moving_rec.y + moving_rec.height / 2};
    if (ray_intersects_rect(ray_origin, vel, expanded, contact_point,
                            contact_normal, contact_time)) {

        if (*contact_time < 1.0f) {
            return true;
        }
    }

    return false;
}

bool resolve_collision(Rectangle moving_rec, Vector2 vel, Rectangle static_rec,
                       Vector2 *resolve_vel) {
    // TODO implement

    return false;
}