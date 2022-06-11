#pragma once

#include <raylib.h>

Camera2D *get_camera();
void center_camera(float x, float y);
void zoom_camera(float zoom);
void lerp_camera(float x, float y, float speed);