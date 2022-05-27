#include <stdio.h>

#include <ferox.h>
#include <flecs.h>
#include <raylib.h>

#include "engine.h"
#include "include/webengine.h"

#define PHYSICS_WORLD_RECTANGLE                                                \
    ((Rectangle){.width = PHYSICS_WORLD_WIDTH, .height = PHYSICS_WORLD_HEIGHT})

#define WORLD_RECTANGLE ((Rectangle){.width = 800, .height = 600})

frWorld *physics_world;

void we_physics_init() {
    printf("physics init\n");
    // Initialize the physics engine
    physics_world = frCreateWorld(
        (Vector2){
            .x = 0,
            .y = 0,
        },
        WORLD_RECTANGLE);
}

void we_physics_update() {
    // Update physics
    frSimulateWorld(physics_world, GetFrameTime());
}

void we_physics_draw_all_bodies() {
    // Draw all bodies
    int count = frGetWorldBodyCount(physics_world);

    for (int i = 0; i < count; i++) {

        frDrawBody(frGetWorldBody(physics_world, i), GREEN);
    }
}

frWorld *we_physics_get_world() {
    return physics_world;
}

void we_physics_add_to_world(frBody *body) {
    frAddToWorld(physics_world, body);
}

void we_physics_body_system(ecs_iter_t *it) {
    we_physics_body *bodies = ecs_term(it, we_physics_body, 1);
    we_transform *trans = ecs_term(it, we_transform, 2);
    ecs_world_t *world = it->world;
    WE_C(we_transform);
    int count = it->count;
    for (int i = 0; i < count; i++) {
        we_physics_body body = bodies[i];
        ecs_id_t entity = it->entities[i];

        // frTransform fr_trans = frGetBodyTransform(body.body);
        Vector2 p = frGetBodyPosition(body.body);
        frShape *sp = frGetBodyShape(body.body);
        if (frIsShapeRectangle(sp)) {
            Vector2 d = frGetRectangleDimensions(sp);
            d = Vector2Scale(d, 0.5);
            p = Vector2Subtract(p, d);
        }
        printf("%d,%d \n", p.x, p.y);
        ecs_set(world, entity, we_transform,
                {.position = frVec2MetersToPixels(p)});
    }
}

void we_physics_body_on_set(ecs_iter_t *it) {
    we_physics_body *bodies = ecs_term(it, we_physics_body, 1);
    int count = it->count;
    for (int i = 0; i < count; i++) {
        we_physics_body body = bodies[i];
        frBody *fr_body = body.body;
        we_physics_add_to_world(fr_body);
    }
}