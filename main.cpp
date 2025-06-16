#include "Game.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tower Defense with Raylib");
    InitAudioDevice(); // Initialisation audio
    SetTargetFPS(60);

    Music music = LoadMusicStream("ost.mp3");
    PlayMusicStream(music);

    Game game(screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        UpdateMusicStream(music); // Met à jour la musique

        game.ProcessInput();
        game.Update(deltaTime);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        game.Draw();
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}