#ifndef TOWER_H
#define TOWER_H

#include "raylib.h"
#include <vector>

struct Enemy; // Forward declaration

struct Tower
{
    Vector2 position;
    float range;
    float damage;
    float cooldown;
    float cooldownTimer;
    int cost;
    Color color;

    Tower();
    Tower(Vector2 pos, float rng = 80.0f, float dmg = 10.0f, float cd = 1.0f);
    void Update(float deltaTime, std::vector<Enemy> &enemies);
    void Attack(std::vector<Enemy> &enemies);
    void Draw(bool showRange = false);
};

#endif