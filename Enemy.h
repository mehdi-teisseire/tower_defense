#ifndef ENEMY_H
#define ENEMY_H

#include <cstddef>
#include "raylib.h"
#include <vector>

struct Enemy
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    int health;
    int maxHealth;
    bool alive;
    size_t currentWaypoint;
    Color color;

    Enemy(Vector2 startPos, float spd = 50.0f);
    void Update(float deltaTime, const std::vector<Vector2> &path);
    void Draw();
    void TakeDamage(int damage);
};

#endif