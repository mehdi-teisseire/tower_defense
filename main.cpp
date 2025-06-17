#include "Game.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tower Defense with Raylib");
    InitAudioDevice(); // begin music
    SetTargetFPS(60);

    Music music = LoadMusicStream("ost.mp3");
    PlayMusicStream(music);

    Game game(screenWidth, screenHeight);

    bool musicMuted = false;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        UpdateMusicStream(music); // uptdate music

        game.ProcessInput();
        game.Update(deltaTime);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        game.Draw();

        // Draw mute/unmute button
        int btnWidth = 40, btnHeight = 40;
        int btnX = screenWidth - btnWidth - 10;
        int btnY = screenHeight - btnHeight - 10;
        Rectangle muteBtn = { (float)btnX, (float)btnY, (float)btnWidth, (float)btnHeight };

        DrawRectangle(btnX, btnY, btnWidth, btnHeight, musicMuted ? GRAY : GREEN);
        if (musicMuted)
            DrawText("🔇", btnX + 8, btnY + 8, 24, WHITE);
        else
            DrawText("🔊", btnX + 8, btnY + 8, 24, WHITE);

        EndDrawing();

        // management of the mute button
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), muteBtn)) {
            musicMuted = !musicMuted;
            if (musicMuted) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}