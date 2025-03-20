#ifndef SCREENDATA_H
#define SCREENDATA_H

#include "raylib.h"
#include <stdbool.h>

#define CELL_SIZE       40

#define SCREEN_WIDTH    799
#define SCREEN_HEIGHT   800

#define ROWS            20
#define COLS            20

#define MAX_ENEMIES 50

#define MAX_TURRETS 50
#define MAX_PROJECTILES 100

typedef struct {
    int gridX;
    int gridY;
    float x;
    float y;
    float range;         // Radius in pixels for turret attack range
    float damage;        // Damage dealt per shot
    float fireRate;      // Shots per second
    float fireTimer;     // Timer to track when turret can fire again
    bool active;         // Is this turret active?
    int targetEnemyIndex; // Index of the enemy this turret is targeting
} Turret;

typedef struct {
    float x, y;          // Current position
    float targetX, targetY; // Target position (not needed for straight shots)
    float speed;         // Speed of projectile
    float damage;        // Damage to inflict on hit
    bool active;         // Is projectile active?
    int targetEnemyIndex; // Enemy that this projectile is targeting
} Projectile;


typedef enum {
    CELL_EMPTY = 0,
    CELL_WALL,
    CELL_TURRET
} CellType;

struct Node;

typedef struct {
    float x;
    float y;
    int gridX;
    int gridY;
    float speed;
    int health;
    bool active;
    Vector2 path[ROWS*COLS];
    int pathLength;
    int currentPathIndex;
} Enemy;

typedef struct Node {
    int x, y;
    bool visited;
    bool obstacle;
    struct Node* parent;
} Node;

typedef struct {
    Node* nodes[ROWS * COLS];
    int top;
} Stack;

typedef struct {
    Node* nodes[ROWS * COLS];
    int front, rear;
} Queue;

extern int playerLives;
extern int playerScore;

extern CellType grid[ROWS][COLS];
extern int cellX;
extern int cellY;
extern Node pathNodes[ROWS][COLS];
extern Enemy enemies[MAX_ENEMIES];
extern int activeEnemies;
extern float spawnTimer;
extern float spawnInterval;

void DrawPreviewPath(void);

void GridDraw(void);
void DrawWalls(void);
void HandleWallPlacement(void);
void SpawnEnemies(void);
void CreateTurret(void);
bool isStackEmpty(Stack* stack);
void initStack(Stack* stack);
void pushStack(Stack* stack, Node* node);
Node* popStack(Stack* stack);
void initPathfindingGrid(void);
void InitEnemies(void);
void SpawnEnemy(void);
void UpdateEnemies(void);
void DrawEnemies(void);
void initQueue(Queue* q);
bool isQueueEmpty(Queue* q);
void enqueue(Queue* q, Node* node);
Node* dequeue(Queue* q);
bool findPathBFS(int startX, int startY, int goalX, int goalY, Vector2 path[], int* pathLength);
void FindPath(Enemy* enemy, int startX, int startY, int goalX, int goalY);

extern Turret turrets[MAX_TURRETS];
extern int activeTurrets;
extern Projectile projectiles[MAX_PROJECTILES];
extern int activeProjectiles;

void InitTurrets(void);
void UpdateTurrets(void);
void DrawTurrets(void);
void FireProjectile(int turretIndex, int enemyIndex);
void UpdateProjectiles(void);
void DrawProjectiles(void);
void CheckProjectileEnemyCollisions(void);

#endif //SCREENDATA_H