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

void GridDraw(void);
void DrawWalls(void);
void HandleWallPlacement(void);
void SpawnEnemies(void);
void CreateTurret(void);
void DrawTurret(void);
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

#endif //SCREENDATA_H