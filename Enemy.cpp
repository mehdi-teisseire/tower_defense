#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Vector2 startPos, float spd)
    : position(startPos), speed(spd), alive(true), currentWaypoint(0), color(RED)
{
    maxHealth = 100;
    health = maxHealth;
}

void Enemy::Update(float deltaTime, const std::vector<Vector2> &path)
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
}

void Enemy::Draw()
{
    if (!alive)
        return;

    DrawCircleV(position, 15, color);
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