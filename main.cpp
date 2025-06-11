#include "Game.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tower Defense with Raylib");
    SetTargetFPS(60);

    Game game(screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        game.ProcessInput();
        game.Update(deltaTime);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}