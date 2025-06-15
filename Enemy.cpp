#include "Game.h"
#include <algorithm>
#include <cmath>     // For sqrt()
#include <cfloat>    // For FLT_MAX
#include <raylib.h>  // For Vector2 and drawing functions
#include <raymath.h> // For Vector2Distance()

Enemy::Enemy(Vector2 startPos, float health, EnemyType type)
    : position(startPos), currentWaypoint(0), speed(100.0f), health(health),
      alive(true), type(type), attackLaserTimer(0.0f), lastAttackTowerIndex(-1)
{
}

void Enemy::Update(float deltaTime, const std::vector<Vector2> &path, const std::vector<Tower> &towers)
{
    if (!alive)
        return;

    if (currentWaypoint < static_cast<int>(path.size()))
    {
        Vector2 target = path[currentWaypoint];
        Vector2 direction = {target.x - position.x, target.y - position.y};
        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.0f)
        {
            currentWaypoint++;
        }
        else
        {
            direction.x /= distance;
            direction.y /= distance;
            position.x += direction.x * speed * deltaTime;
            position.y += direction.y * speed * deltaTime;
        }
    }

    // Handle attack behavior if this is an attack enemy
    if (type == ATTACK)
    {
        attackLaserTimer -= deltaTime;
        if (attackLaserTimer <= 0.0f)
        {
            // Find closest tower to attack
            float minDist = FLT_MAX;
            int closestTower = -1;

            for (size_t i = 0; i < towers.size(); ++i)
            {
                if (towers[i].destroyed)
                    continue;

                float dist = Vector2Distance(position, towers[i].position);
                if (dist < minDist)
                {
                    minDist = dist;
                    closestTower = static_cast<int>(i);
                }
            }

            if (closestTower != -1 && minDist < 150.0f) // Attack range
            {
                lastAttackTowerIndex = closestTower;
                attackLaserTimer = 1.0f; // Cooldown
            }
        }
    }
}

void Enemy::Draw() const
{
    Color color = RED; // Default for normal enemies
    if (type == ATTACK)
        color = ORANGE;
    else if (type == BOSS)
        color = PURPLE;

    DrawCircleV(position, 20, color);
    DrawCircleLines(position.x, position.y, 20, BLACK);

    // Health bar
    DrawRectangle(position.x - 20, position.y - 30, 40, 5, GRAY);
    DrawRectangle(position.x - 20, position.y - 30, 40 * (health / 50.0f), 5, GREEN);

    // Draw attack laser if active
    if (type == ATTACK && attackLaserTimer > 0.0f && lastAttackTowerIndex != -1)
    {
        DrawLineEx(position, position, 3, YELLOW);
    }
}