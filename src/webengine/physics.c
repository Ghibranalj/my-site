#include <stdio.h>

#include "include/webengine.h"
#include <flecs.h>
#include <raylib.h>

// #define PHYSAC_AVOID_TIMMING_SYSTEM
#define __linux__
#define PHYSAC_DEBUG
#define PHYSAC_IMPLEMENTATION
#include <physac.h>

void we_physics_body_transform_system(ecs_iter_t *it) {
    we_physics_body *bodies = ecs_term(it, we_physics_body, 1);
    we_transform *transforms = ecs_term(it, we_transform, 2);

    for (int i = 0; i < it->count; i++) {
        we_physics_body body = bodies[i];
        we_transform transform = transforms[i];

        ecs_id_t entity = it->entities[i];

        WE_C(we_transform);
        ecs_set(it->world, entity, we_transform,
                {
                    .position = body.body->position,
                });
    }
}

void we_draw_physics_body(PhysicsBody body) {
    int vertexCount = GetPhysicsShapeVerticesCount(body->id);
    for (int j = 0; j < vertexCount; j++) {

        Vector2 vertexA = GetPhysicsShapeVertex(body, j);

        int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);
        Vector2 vertexB = GetPhysicsShapeVertex(body, jj);
        DrawLineV(vertexA, vertexB, GREEN);
    }
}

// void we_add_physics_body(ecs_id_t entity, float width, float height) {}