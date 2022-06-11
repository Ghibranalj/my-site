
#pragma once
#include <raylib.h>

bool resolve_collision(Rectangle moving_rec, Vector2 vel, Rectangle static_rec,
                       Vector2 *resolve_vel);

bool DynamicRectVsRect(Rectangle moving_rec, Vector2 vel, Rectangle static_rec,
                       Vector2 *contact_point, Vector2 *contact_normal,
                       float *contact_time);
