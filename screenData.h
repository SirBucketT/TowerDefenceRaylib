//
// Created by theodor on 2025-03-13.
//

#ifndef SCREENDATA_H
#define SCREENDATA_H

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

extern int playerLives();
int playerScore = 100;

extern CellType grid[ROWS][COLS];
extern int cellX;
extern int cellY;

void GridDraw(void);
void DrawWalls(void);
void HandleWallPlacement(void);
void SpawnEnemies();
void CreateTurret();
void DrawTurret();
void InitEnemies(void);
void UpdateEnemies(void);
void DrawEnemies(void);
void FindPath(Enemy* enemy, int startX, int startY, int goalX, int goalY);

#endif //SCREENDATA_H
