#include "Game.h"
#include <algorithm>
#include <cfloat>
#include <raymath.h>

Tower::Tower(Vector2 pos, float range, float damage, float fireRate, TowerType type)
    : position(pos), range(range), damage(damage), fireRate(fireRate),
      fireTimer(0.0f), type(type), destroyed(false), cost(100)
{
    if (type == LASER)
        cost = 150;
    else if (type == POISON)
        cost = 200;
}

void Tower::Update(float deltaTime, std::vector<Enemy> &enemies)
{
    if (destroyed)
        return;

    fireTimer -= deltaTime;

    if (fireTimer <= 0.0f)
    {
        // Find closest enemy in range
        float minDist = FLT_MAX;
        Enemy *target = nullptr;

        for (auto &enemy : enemies)
        {
            if (!enemy.alive)
                continue;

            float dist = Vector2Distance(position, enemy.position);
            if (dist < range && dist < minDist)
            {
                minDist = dist;
                target = &enemy;
            }
        }

        if (target)
        {
            target->health -= damage;
            fireTimer = fireRate;

            // Special effects based on tower type
            if (type == POISON)
            {
                // Area damage
                for (auto &enemy : enemies)
                {
                    if (!enemy.alive)
                        continue;
                    if (Vector2Distance(position, enemy.position) < range * 0.6f)
                    {
                        enemy.health -= damage * 0.5f;
                    }
                }
            }
        }
    }
}

void Tower::Draw(bool ghost) const
{
    if (destroyed && !ghost)
        return;

    Color color = BLUE;
    if (type == LASER)
        color = SKYBLUE;
    else if (type == POISON)
        color = LIME;

    if (ghost)
    {
        color.a = 150; // Semi-transparent
        DrawCircleV(position, range, Fade(color, 0.1f));
    }

    DrawCircleV(position, 25, color);
    DrawCircleLines(position.x, position.y, 25, BLACK);

    float healthRatio = (float)health / 100.0f;
    DrawRectangle(position.x - 20, position.y - 28, 40, 5, DARKGRAY);
    DrawRectangle(position.x - 20, position.y - 28, 40 * healthRatio, 5, GREEN);

    if (!ghost)
    {
        DrawCircleLines(position.x, position.y, range, Fade(color, 0.3f));
    }
}

void Tower::TakeDamage(int dmg)
{
    health -= dmg;
    if (health <= 0) destroyed = true;
}