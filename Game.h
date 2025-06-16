#ifndef GAME_H
#define GAME_H

#include <vector>
#include <raylib.h>

enum GameState
{
    TITLE_SCREEN,
    GAMEPLAY,
    GAME_OVER
};

enum EnemyType
{
    NORMAL,
    ATTACK,
    BOSS
};

enum TowerType
{
    BASIC,
    LASER,
    POISON
};

class Enemy;

class Tower
{
public:
    Vector2 position;
    float range;
    float damage;
    float fireRate;
    float fireTimer;
    TowerType type;
    bool destroyed;
    int cost;
    int health = 100;

    Tower(Vector2 pos, float range, float damage, float fireRate, TowerType type);
    void Update(float deltaTime, std::vector<Enemy> &enemies);
    void Draw(bool ghost = false) const;
    void TakeDamage(int dmg);
};

class Enemy
{
public:
    Vector2 position;
    int currentWaypoint;
    float speed;
    float cooldown;
    float cooldownTimer;
    float health;
    float maxHealth;
    bool alive;
    EnemyType type;
    float attackLaserTimer;
    int lastAttackTowerIndex;

    Enemy(Vector2 startPos, float health, EnemyType type);
    void Update(float deltaTime, const std::vector<Vector2> &path, std::vector<Tower>& towers);
    void Draw() const;
};

class Game
{
public:
    Game(int width, int height);
    ~Game();

    void LoadResources();
    void UnloadResources();
    void ResetGame();
    void Update(float deltaTime);
    void Draw();
    void ProcessInput();

private:
    void DrawTitleScreen();
    void DrawGameOverScreen();
    bool IsOnPath(Vector2 point) const;

    int money;
    int lives;
    float enemySpawnTimer;
    float enemySpawnInterval;
    bool placingTower;
    bool gameOver;
    int screenWidth;
    int screenHeight;
    GameState currentState;
    bool waitingForMouseRelease;
    bool showTowerMenu;
    TowerType towerTypeToBuild;

    std::vector<Vector2> path;
    std::vector<Enemy> enemies;
    std::vector<Tower> towers;
    Tower tempTower;

    // Resources
    Texture2D backgroundTexture;
    Texture2D titleTexture;
    Sound titleMusic;
};

#endif // GAME_H