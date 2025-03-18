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
    int x, y;
    bool visited;
    bool obstacle;
    struct Node* parent;
} Node;

typedef struct {
    Node* nodes [ROWS][COLS];
    int top;
}Stack;

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
void initStack(Stack* stack);

#endif //SCREENDATA_H
