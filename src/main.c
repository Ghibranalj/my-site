
#include "flecs.h"
#include "raylib.h"

typedef struct {
    int x;
    int y;
} Position;

void Move(ecs_iter_t *it) {
    Position *p = ecs_term(it, Position, 1);

    for (int i = 0; i < it->count; i++) {
        p[i].x += 5;
        p[i].y += 5; 
    }
}

void Render(ecs_iter_t *it) {
    Position *p = ecs_term(it, Position, 1);

    for (int i = 0; i < it->count; i++) {
        DrawRectangle(p[i].x, p[i].y, 10, 10, BLUE);
    }
}
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight,
               "Test Window");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    ecs_world_t *world = ecs_init();

    InitAudioDevice();

    Sound country = LoadSound("resources/country.mp3");

    ECS_COMPONENT(world, Position);

    ECS_SYSTEM(world, Move, EcsOnUpdate, Position);
    ECS_SYSTEM(world, Render, EcsOnUpdate, Position);

    ecs_entity_t * e = ecs_new_entity(world,"test");
    ecs_add(world, e, Position);
    ecs_set(world, e, Position, {.x = 10, .y = 10});
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(country);
        }
        DrawText("Press Space for some music!", 190, 200, 20, LIGHTGRAY);
        ecs_progress(world, GetTime());
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}