#include "Tower.h"
#include "Enemy.h"
#include <cmath>

Tower::Tower()
    : position({0, 0}), range(80.0f), damage(10.0f), cooldown(1.0f),
      cooldownTimer(0), cost(100), color(BLUE) {}

Tower::Tower(Vector2 pos, float rng, float dmg, float cd)
    : position(pos), range(rng), damage(dmg), cooldown(cd),
      cooldownTimer(0), cost(100), color(BLUE) {}

void Tower::Update(float deltaTime, std::vector<Enemy> &enemies)
{
    cooldownTimer -= deltaTime;
    if (cooldownTimer <= 0)
    {
        Attack(enemies);
        cooldownTimer = cooldown;
    }
}

void Tower::Attack(std::vector<Enemy> &enemies)
{
    for (auto &enemy : enemies)
    {
        if (!enemy.alive)
            continue;

        float dx = enemy.position.x - position.x;
        float dy = enemy.position.y - position.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= range)
        {
            enemy.TakeDamage(damage);
            DrawLineV(position, enemy.position, YELLOW);
            break;
        }
    }
}

void Tower::Draw(bool showRange)
{
    DrawCircleV(position, 20, color);
    if (showRange)
    {
        DrawCircleLines(position.x, position.y, range, Fade(BLUE, 0.5f));
    }
}