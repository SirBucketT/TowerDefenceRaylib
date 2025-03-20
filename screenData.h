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
    float range;
    float damage;
    float fireRate;
    float fireTimer;
    bool active;
    int targetEnemyIndex;
} Turret;

typedef struct {
    float x, y;
    float targetX, targetY;
    float speed;
    float damage;
    bool active;
    int targetEnemyIndex;
} Projectile;

typedef enum {
    CELL_EMPTY = 0,
    CELL_WALL,
    CELL_TURRET,
    CELL_WATER,
    CELL_DIRT,
    CELL_GRASS
} CellType;

struct Node;

typedef struct {
    float x;
    float y;
    int gridX;
    int gridY;
    float speed;
    float baseSpeed;
    int health;
    bool active;
    Vector2 path[ROWS*COLS];
    int pathLength;
    int currentPathIndex;
    bool isWet;
    float wetTimer;
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