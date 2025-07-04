#include "Game.h"
#include <algorithm>
#include <cmath>

Game::Game(int width, int height)
    : money(3000), lives(10), enemySpawnTimer(0), enemySpawnInterval(2.0f),
      placingTower(false), gameOver(false), screenWidth(width), screenHeight(height),
      currentState(TITLE_SCREEN), waitingForMouseRelease(false), showTowerMenu(false),
      tempTower({0, 0}, 80, 10, 1.0f, BASIC)
{
    // Initialize path
    path.push_back({0.0f, (float)height / 2});
    path.push_back({200.0f, (float)height / 2});
    path.push_back({200.0f, 150.0f});
    path.push_back({400.0f, 150.0f});
    path.push_back({400.0f, 450.0f});
    path.push_back({600.0f, 450.0f});
    path.push_back({600.0f, (float)height / 2});
    path.push_back({(float)width, (float)height / 2});

    // Load resources
    LoadResources();
}

void Game::LoadResources()
{
    // Load textures (replace with your actual files)
    // backgroundTexture = LoadTexture("resources/background.png");
    // titleTexture = LoadTexture("resources/title.png");

    // Load sound
    // titleMusic = LoadSound("resources/title_music.mp3");
}

void Game::UnloadResources()
{
    // Unload textures
    // UnloadTexture(backgroundTexture);
    // UnloadTexture(titleTexture);

    // Unload sound
    // UnloadSound(titleMusic);
}

void Game::ResetGame()
{
    money = 1200;
    lives = 10;
    enemies.clear();
    towers.clear();
    placingTower = false;
    enemySpawnTimer = 0;
    gameOver = false;
    showTowerMenu = false;
    waitingForMouseRelease = false;
}

void Game::DrawTitleScreen()
{
    ClearBackground(DARKBLUE);

    // Draw title
    DrawText("TOWER DEFENSE",
             screenWidth / 2 - MeasureText("TOWER DEFENSE", 50) / 2,
             screenHeight / 4,
             50, WHITE);

    // Draw instructions
    DrawText("Defend your base against enemy waves",
             screenWidth / 2 - MeasureText("Defend your base against enemy waves", 30) / 2,
             screenHeight / 2 - 60,
             30, WHITE);

    // Draw tower types info
    DrawText("Tower Types:", screenWidth / 2 - 100, screenHeight / 2, 25, YELLOW);
    DrawText("- Basic: Standard tower", screenWidth / 2 - 150, screenHeight / 2 + 30, 20, LIGHTGRAY);
    DrawText("- Laser: Fast attacks", screenWidth / 2 - 150, screenHeight / 2 + 60, 20, LIGHTGRAY);
    DrawText("- Poison: Area damage", screenWidth / 2 - 150, screenHeight / 2 + 90, 20, LIGHTGRAY);

    // Draw start prompt
    DrawText("Press SPACE to Start",
             screenWidth / 2 - MeasureText("Press SPACE to Start", 30) / 2,
             screenHeight * 3 / 4,
             30, GREEN);
}

void Game::DrawGameOverScreen()
{
    ClearBackground(BLACK);

    DrawText("GAME OVER",
             screenWidth / 2 - MeasureText("GAME OVER", 50) / 2,
             screenHeight / 3,
             50, RED);

    DrawText(TextFormat("Final Score: %d", money),
             screenWidth / 2 - MeasureText(TextFormat("Final Score: %d", money), 30) / 2,
             screenHeight / 2,
             30, WHITE);

    DrawText("Press SPACE to Play Again",
             screenWidth / 2 - MeasureText("Press SPACE to Play Again", 30) / 2,
             screenHeight * 2 / 3,
             30, GREEN);
}

void Game::Update(float deltaTime)
{
    switch (currentState)
    {
    case TITLE_SCREEN:
        // No updates needed for title screen
        break;

    case GAMEPLAY:
        if (gameOver)
        {
            currentState = GAME_OVER;
            break;
        }

        // Spawn enemies
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= enemySpawnInterval)
        {
            int typeRand = GetRandomValue(0, 9);
            EnemyType type = NORMAL;
            if (typeRand < 6)
                type = NORMAL; // 60% chance
            else if (typeRand < 9)
                type = ATTACK; // 30% chance
            else
                type = BOSS; // 10% chance
            
            if (type == NORMAL)
                enemies.emplace_back(path[0], 80.0f, type); // Normal enemies spawn every 2 seconds
            else if (type == ATTACK)
                enemies.emplace_back(path[0], 50.0f, type); // Attack enemies spawn every 3 seconds
            else
                enemies.emplace_back(path[0], 200.0f, type); // Boss enemies spawn every 5 seconds
            enemySpawnTimer = 0;
        }

        // Update enemies
        for (auto &enemy : enemies)
        {
            enemy.Update(deltaTime, path, towers);

            // Check if enemy is dead from damage
            if (enemy.health <= 0 && enemy.alive)
            {
                enemy.alive = false;
                enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                             [](const Enemy &e)
                                             { return !e.alive; }),
                              enemies.end());
                money += 100;
            }

            if (enemy.currentWaypoint >= static_cast<int>(path.size()) && enemy.alive)
            {
                lives--;
                enemy.alive = false;
                if (lives <= 0)
                {
                    gameOver = true;
                }
            }
        }

        // Update towers
        for (auto &tower : towers)
        {
            if (!tower.destroyed)
                tower.Update(deltaTime, enemies);
        }

        // Tower placement
        if (placingTower)
        {
            tempTower.position = GetMousePosition();

            if (waitingForMouseRelease)
            {
                if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    waitingForMouseRelease = false;
                }
                return;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (money >= tempTower.cost && !IsOnPath(tempTower.position))
                {
                    towers.push_back(tempTower);
                    money -= tempTower.cost;
                }
                placingTower = false;
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                placingTower = false;
            }
        }

        if (showTowerMenu)
        {
            Vector2 mousePos = GetMousePosition();

            Rectangle basicBtn = {700, 50, 80, 30};
            Rectangle laserBtn = {700, 90, 80, 30};
            Rectangle poisonBtn = {700, 130, 80, 30};

            if (CheckCollisionPointRec(mousePos, basicBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                towerTypeToBuild = BASIC;
                placingTower = true;
                waitingForMouseRelease = true;
                tempTower = Tower({0, 0}, 80, 10, 1.0f, BASIC);
                showTowerMenu = false;
            }
            if (CheckCollisionPointRec(mousePos, laserBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                towerTypeToBuild = LASER;
                placingTower = true;
                waitingForMouseRelease = true;
                tempTower = Tower({0, 0}, 100, 5, 0.2f, LASER);
                showTowerMenu = false;
            }
            if (CheckCollisionPointRec(mousePos, poisonBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                towerTypeToBuild = POISON;
                placingTower = true;
                waitingForMouseRelease = true;
                tempTower = Tower({0, 0}, 70, 2, 1.5f, POISON);
                showTowerMenu = false;
            }
        }
        break;

    case GAME_OVER:
        // No updates needed for game over screen
        break;
    }
}

void Game::Draw()
{
    switch (currentState)
    {
    case TITLE_SCREEN:
        DrawTitleScreen();
        break;

    case GAMEPLAY:
        // Draw path
        for (size_t i = 1; i < path.size(); ++i)
        {
            DrawLineV(path[i - 1], path[i], YELLOW);
        }

        // Draw enemies
        for (auto &enemy : enemies)
        {
            enemy.Draw();
        }

        // Draw towers
        for (auto &tower : towers)
        {
            if (!tower.destroyed)
                tower.Draw();
        }

        // Draw tower being placed
        if (placingTower)
        {
            tempTower.Draw(true);
        }

        // Draw UI
        DrawText(TextFormat("Money: %d", money), 10, 10, 20, WHITE);
        DrawText(TextFormat("Lives: %d", lives), 10, 40, 20, WHITE);

        // Draw build tower button
        DrawRectangle(700, 10, 80, 30, GREEN);
        DrawText("Build Tower", 705, 15, 12, BLACK);

        if (showTowerMenu)
        {
            DrawRectangle(700, 50, 80, 30, LIGHTGRAY);
            DrawText("Basic", 705, 55, 12, BLACK);
            DrawRectangle(700, 90, 80, 30, LIGHTGRAY);
            DrawText("Laser", 705, 95, 12, BLACK);
            DrawRectangle(700, 130, 80, 30, LIGHTGRAY);
            DrawText("Poison", 705, 135, 12, BLACK);
        }

        for (const auto &enemy : enemies)
        {
            if (enemy.type == ATTACK && enemy.lastAttackTowerIndex != -1 && enemy.attackLaserTimer > 0.0f)
            {
                const auto &tower = towers[enemy.lastAttackTowerIndex];
                if (!tower.destroyed)
                {
                    DrawLineEx(enemy.position, tower.position, 4, RED);
                }
            }
        }
        break;

    case GAME_OVER:
        DrawGameOverScreen();
        break;
    }
}

void Game::ProcessInput()
{
    switch (currentState)
    {
    case TITLE_SCREEN:
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            ResetGame();
            currentState = GAMEPLAY;
        }
        break;
    }

    case GAMEPLAY:
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            placingTower = false;
            waitingForMouseRelease = false;
        }

        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, {700, 10, 80, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            showTowerMenu = true;
            placingTower = false;
            waitingForMouseRelease = false;
        }
        break;
    }

    case GAME_OVER:
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            ResetGame();
            currentState = GAMEPLAY;
        }
        break;
    }
    }
}

bool Game::IsOnPath(Vector2 point) const
{
    for (size_t i = 1; i < path.size(); ++i)
    {
        Vector2 a = path[i - 1];
        Vector2 b = path[i];

        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float length2 = dx * dx + dy * dy;
        float t = ((point.x - a.x) * dx + (point.y - a.y) * dy) / length2;
        t = fmaxf(0, fminf(1, t));
        Vector2 proj = {a.x + t * dx, a.y + t * dy};
        float dist2 = (point.x - proj.x) * (point.x - proj.x) + (point.y - proj.y) * (point.y - proj.y);
        if (dist2 < (50 / 2) * (50 / 2))
            return true;
    }
    return false;
}
Game::~Game()
{
    UnloadResources();
}
