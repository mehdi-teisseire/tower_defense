#include "Tower.h"
#include "Enemy.h"
#include <cmath>

Tower::Tower()
    : position({0, 0}), range(80.0f), damage(15.0f), health(100), destroyed(false),
      cooldown(1.0f), cooldownTimer(0), cost(100), color(BLUE), type(BASIC) {}

Tower::Tower(Vector2 pos, float rng, float dmg, float cd, TowerType t)
    : position(pos), range(rng), damage(dmg), health(100), destroyed(false),
      cooldown(cd), cooldownTimer(0), cost(100), type(t)
{
    switch (type) {
        case BASIC:  color = BLUE;  break;
        case LASER:  color = RED;   break;
        case POISON: color = GREEN; break;
        default:     color = BLUE;  break;
    }
}

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
    switch (type) {
    case BASIC:
        // Attack 
        for (auto &enemy : enemies) {
            if (!enemy.alive) continue;
            float dx = enemy.position.x - position.x;
            float dy = enemy.position.y - position.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= range) {
                enemy.TakeDamage(damage);
                DrawLineV(position, enemy.position, YELLOW);
                break;
            }
        }
        break;
    case LASER:
        // laser attack 
        for (auto &enemy : enemies) {
            if (!enemy.alive) continue;
            float dx = enemy.position.x - position.x;
            float dy = enemy.position.y - position.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= range) {
                enemy.TakeDamage(damage * 0.7f);
                DrawLineV(position, enemy.position, RED);
                
            }
        }
        break;
    case POISON:
        // Poison attack
        for (auto &enemy : enemies) {
            if (!enemy.alive) continue;
            float dx = enemy.position.x - position.x;
            float dy = enemy.position.y - position.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= range) {
                enemy.TakeDamage(damage * 0.2f);
                enemy.ApplyPoison(3.0f, 5.0f);
                DrawLineV(position, enemy.position, GREEN);
                break;
            }
        }
        break;
    }
}

void Tower::TakeDamage(int dmg)
{
    health -= dmg;
    if (health <= 0) destroyed = true;
}

void Tower::Upgrade()
{
    if (upgraded) return; // Already upgraded

    switch (type) {
        case BASIC:
            damage += 10.0f;
            cooldown -= 0.4f;
            health += 50;
            maxHealth += 50;
            break;
        case LASER:
            damage += 5.0f;
            health += 50;
            maxHealth += 50;
            break;
        case POISON:
            damage += 4.0f;
            health += 50;
            maxHealth += 50;
            break;
    }
    upgraded = true;
}

void Tower::Draw(bool showRange)
{
    if (upgraded) {
        // Draw triangle
        Vector2 p1 = {position.x, position.y - 24};
        Vector2 p2 = {position.x - 20, position.y + 20};
        Vector2 p3 = {position.x + 20, position.y + 20};
        DrawTriangle(p1, p2, p3, color);
    } else {
        // Draw square
        DrawRectangleV({position.x - 20, position.y - 20}, {40, 40}, color);
    }

    float healthRatio = (float)health / (float)maxHealth;
    DrawRectangle(position.x - 20, position.y - 28, 40, 5, BLACK);
    DrawRectangle(position.x - 20, position.y - 28, 40 * healthRatio, 5, GREEN);

    if (showRange)
    {
        DrawCircleLines(position.x, position.y, range, Fade(BLUE, 0.5f));
    }
}