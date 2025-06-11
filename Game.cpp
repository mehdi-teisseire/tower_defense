#include "Game.h"
#include <algorithm>

Game::Game(int width, int height)
    : money(3000), lives(10), enemySpawnTimer(0), enemySpawnInterval(2.0f),
      placingTower(false), gameOver(false), screenWidth(width), screenHeight(height)
{

    path.push_back({0.0f, (float)height / 2});
    path.push_back({200.0f, (float)height / 2});
    path.push_back({200.0f, 150.0f});
    path.push_back({400.0f, 150.0f});
    path.push_back({400.0f, 450.0f});
    path.push_back({600.0f, 450.0f});
    path.push_back({600.0f, (float)height / 2});
    path.push_back({(float)width, (float)height / 2});
}

void Game::Update(float deltaTime)
{
    if (gameOver)
        return;

    // Spawn enemies
    enemySpawnTimer += deltaTime;
    if (enemySpawnTimer >= enemySpawnInterval)
    {
        enemies.emplace_back(path[0]);
        enemySpawnTimer = 0;
    }

    // Update enemies
    for (auto &enemy : enemies)
    {
        enemy.Update(deltaTime, path);

        if (enemy.currentWaypoint >= path.size() && enemy.alive)
        {
            lives--;
            enemy.alive = false;
            if (lives <= 0)
            {
                gameOver = true;
            }
        }
    }

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const Enemy &e)
                                 { return !e.alive; }),
                  enemies.end());

    // Update towers
    for (auto &tower : towers)
    {
        tower.Update(deltaTime, enemies);
    }

    // Tower placement
    if (placingTower)
    {
        tempTower.position = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (money >= tempTower.cost)
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
}

void Game::Draw()
{
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

    if (gameOver)
    {
        DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 25, 50, RED);
    }
}

void Game::ProcessInput()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        placingTower = false;
    }

    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, {700, 10, 80, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        placingTower = true;
        tempTower = Tower({0, 0});
    }
}