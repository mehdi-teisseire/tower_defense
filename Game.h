#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>
#include "Enemy.h"
#include "Tower.h"

class Game
{
private:
    std::vector<Vector2> path;
    std::vector<Enemy> enemies;
    std::vector<Tower> towers;

    int money;
    int lives;
    float enemySpawnTimer;
    float enemySpawnInterval;
    bool placingTower;
    Tower tempTower;
    bool gameOver;
    int screenWidth;
    int screenHeight;

public:
    Game(int width, int height);
    void Update(float deltaTime);
    void Draw();
    void ProcessInput();
};

#endif