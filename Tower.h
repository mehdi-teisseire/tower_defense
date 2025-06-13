#ifndef TOWER_H
#define TOWER_H

#include "raylib.h"
#include <vector>

struct Enemy; // Forward declaration

enum TowerType { BASIC, LASER, POISON };

struct Tower
{
    Vector2 position;
    float range;
    float damage;
    int health = 100; 
    bool destroyed;
    float cooldown;
    float cooldownTimer;
    int cost;
    Color color;
    TowerType type;

    Tower();
    Tower(Vector2 pos, float rng = 80.0f, float dmg = 10.0f, float cd = 1.0f, TowerType t = BASIC);
    void Update(float deltaTime, std::vector<Enemy> &enemies);
    void Attack(std::vector<Enemy> &enemies);
    void TakeDamage(int dmg);
    void Draw(bool showRange = false);
};

#endif