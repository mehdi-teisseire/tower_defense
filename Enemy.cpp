#include "Enemy.h"
#include "Tower.h"
#include <cmath>

Enemy::Enemy(Vector2 startPos, float spd, EnemyType t, const std::vector<Vector2>& pathToFollow)
    : position(startPos), speed(spd), cooldown(0.6f), cooldownTimer(0), alive(true), currentWaypoint(0), type(t), path(pathToFollow)
{
    switch (type) {
        case NORMAL:
            health = maxHealth = 100;
            color = RED;
            break;
        case ATTACK:
            health = maxHealth = 60;
            cooldown = 1.0f;
            color = GRAY;
            break;
        case BOSS:
            health = maxHealth = 300;
            speed = 30.0f;
            color = BLACK;
            break;
    }
}

void Enemy::Update(float deltaTime, std::vector<Tower>& towers)
{
    if (!alive || path.empty())
        return;

    if (currentWaypoint < path.size())
    {
        Vector2 target = path[currentWaypoint];
        Vector2 direction = {target.x - position.x, target.y - position.y};
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.0f)
        {
            currentWaypoint++;
        }
        else
        {
            if (distance > 0)
            {
                direction.x /= distance;
                direction.y /= distance;
            }
            velocity.x = direction.x * speed;
            velocity.y = direction.y * speed;
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
    }

    cooldownTimer -= deltaTime;

    if (type == ATTACK) {
        lastAttackTowerIndex = -1;
        cooldownTimer -= deltaTime;
        if (cooldownTimer <= 0.0f) {
            for (size_t i = 0; i < towers.size(); ++i) {
                auto& tower = towers[i];
                if (tower.destroyed) continue;
                float dx = tower.position.x - position.x;
                float dy = tower.position.y - position.y;
                float dist = sqrt(dx * dx + dy * dy);
                if (dist < 70.0f) {
                    tower.TakeDamage(20);
                    lastAttackTowerIndex = (int)i;
                    attackLaserTimer = 0.1f; 
                    cooldownTimer = cooldown;
                    break;
                }
            }
        } else {
            attackLaserTimer -= deltaTime;
            if (attackLaserTimer < 0.0f) attackLaserTimer = 0.0f;
            lastAttackTowerIndex = -1;
        }
    }

    if (poisoned)
    {
        poisonTimer += deltaTime;
        poisonTick += deltaTime;
        if (poisonTick >= 1.0f) // Applique les dégâts chaque seconde
        {
            TakeDamage((int)poisonDamagePerSecond);
            poisonTick = 0.0f;
        }
        if (poisonTimer >= poisonDuration + 2.5f)
        {
            poisoned = false;
            poisonTimer = 0.0f;
            poisonTick = 0.0f;
            poisonDamagePerSecond = 0.0f;
            poisonDuration = 0.0f;
        }
    }
}

void Enemy::Draw()
{
    if (!alive)
        return;

    Color drawColor = poisoned ? GREEN : color;
    DrawCircleV(position, 15, drawColor);
    float healthRatio = (float)health / maxHealth;
    DrawRectangle(position.x - 15, position.y - 25, 30 * healthRatio, 5, GREEN);
}

void Enemy::TakeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        alive = false;
    }
}

void Enemy::ApplyPoison(float duration, float dps)
{
    poisoned = true;
    poisonDuration = duration;
    poisonTimer = 0.0f;
    poisonTick = 0.0f;
    poisonDamagePerSecond = dps;
}