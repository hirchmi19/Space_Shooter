//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"

int main()
{
    InitWindow(800, 600, "Space Shooter");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("It works!", 300, 280, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}