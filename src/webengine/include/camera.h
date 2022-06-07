#pragma once

#include <raylib.h>

Camera2D *we_get_camera();
void we_center_camera(float x, float y);
void we_zoom_camera(float zoom);
void we_lerp_camera(float x, float y, float speed);