#include <raylib.h>

Vector2 we_v2_lerp(Vector2 a, Vector2 b, float l) {
    Vector2 out;
    out.x = (1.0 - l) * a.x + l * b.x;
    out.y = (1.0 - l) * a.y + l * b.y;
    return out;
}