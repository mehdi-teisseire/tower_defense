#ifndef ENEMY_H
#define ENEMY_H

#include <cstddef>
#include "raylib.h"
#include <vector>

enum EnemyType { NORMAL, ATTACK, BOSS };

struct Tower;

class Enemy {
public:
    Vector2 position;
    Vector2 velocity;
    float speed;
    float cooldown;
    float cooldownTimer;
    int health;
    int maxHealth;
    bool alive;
    size_t currentWaypoint;
    Color color;
    EnemyType type;
    int lastAttackTowerIndex = -1;

    bool poisoned = false;
    float poisonTimer = 0.0f;
    float poisonTick = 0.0f;
    float poisonDuration = 0.0f;
    float poisonDamagePerSecond = 0.0f;
    float attackLaserTimer = 0.0f;

    std::vector<Vector2> path; // Chemin propre à cet ennemi

    Enemy(Vector2 startPos, float spd, EnemyType t, const std::vector<Vector2>& pathToFollow);
    void Update(float deltaTime, std::vector<Tower>& towers);
    void Draw();
    void TakeDamage(int damage);
    void ApplyPoison(float duration, float dps);
};

#endif