#include "screenData.h"
#include "raylib.h"
#include <math.h>

CellType grid[ROWS][COLS] = {0};
int cellX;
int cellY;
Node pathNodes[ROWS][COLS];
Enemy enemies[MAX_ENEMIES];
int activeEnemies = 0;
float spawnTimer = 0.0f;
float spawnInterval = 0.3f;
int playerLives = 3;
int playerScore = 0;
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

bool isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void GridDraw() {
    for (int i = 0; i <= COLS; i++) {
        DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT, WHITE);
    }
    for (int i = 0; i <= ROWS; i++) {
        DrawLine(0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE, WHITE);
    }
}

void DrawWalls() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == CELL_WALL) {
                DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, GRAY);
            }
        }
    }
}

void HandleWallPlacement() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();

        cellX = getLeMousePosition.x / CELL_SIZE;
        cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            grid[cellY][cellX] = CELL_WALL;
        }
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();
        cellX = getLeMousePosition.x / CELL_SIZE;
        cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            grid[cellY][cellX] = CELL_EMPTY;
        }
    }
}

void DrawTurret() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == CELL_TURRET) {
                DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, RED);
            }
        }
    }
}

void CreateTurret() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();

        cellX = getLeMousePosition.x / CELL_SIZE;
        cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            if (grid[cellY][cellX] == CELL_EMPTY) {
                grid[cellY][cellX] = CELL_TURRET;
            }
        }
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();
        cellX = getLeMousePosition.x / CELL_SIZE;
        cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            if (grid[cellY][cellX] == CELL_TURRET) {
                grid[cellY][cellX] = CELL_EMPTY;
            }
        }
    }
}

void initStack(Stack* stack) {
    stack->top = -1;
}

void pushStack(Stack* stack, Node* node) {
    stack->top++;
    stack->nodes[stack->top] = node;
}

Node* popStack(Stack* stack) {
    if (isStackEmpty(stack)) {
        return NULL;
    }

    Node* node = stack->nodes[stack->top];
    stack->top--;
    return node;
}

void initPathfindingGrid() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            pathNodes[y][x].x = x;
            pathNodes[y][x].y = y;
            pathNodes[y][x].visited = false;
            pathNodes[y][x].parent = NULL;
            pathNodes[y][x].obstacle = (grid[y][x] == CELL_WALL || grid[y][x] == CELL_TURRET);
        }
    }
}

bool findPathDFS(int startX, int startY, int goalX, int goalY, Vector2 path[], int* pathLength) {
    initPathfindingGrid();

    Stack stack;
    initStack(&stack);

    pushStack(&stack, &pathNodes[startY][startX]);

    pathNodes[startY][startX].visited = true;

    bool foundPath = false;

    while (!isStackEmpty(&stack) && !foundPath) {
        Node* currentNode = popStack(&stack);

        if (currentNode->x == goalX && currentNode->y == goalY) {
            foundPath = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newX = currentNode->x + dx[i];
            int newY = currentNode->y + dy[i];

            if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS) {
                if (!pathNodes[newY][newX].obstacle && !pathNodes[newY][newX].visited) {
                    pathNodes[newY][newX].visited = true;

                    pathNodes[newY][newX].parent = currentNode;

                    pushStack(&stack, &pathNodes[newY][newX]);
                }
            }
        }
    }

    if (foundPath) {
        Node* pathNode = &pathNodes[goalY][goalX];
        int pathIndex = 0;

        Vector2 tempPath[ROWS * COLS];

        while (pathNode != NULL) {
            tempPath[pathIndex].x = pathNode->x * CELL_SIZE + CELL_SIZE / 2;
            tempPath[pathIndex].y = pathNode->y * CELL_SIZE + CELL_SIZE / 2;
            pathIndex++;

            pathNode = pathNode->parent;
        }

        for (int i = 0; i < pathIndex; i++) {
            path[i] = tempPath[pathIndex - 1 - i];
        }

        *pathLength = pathIndex;
        return true;
    }

    *pathLength = 0;
    return false;
}

void InitEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].health = 100;
        enemies[i].speed = 100.0f;
        enemies[i].pathLength = 0;
        enemies[i].currentPathIndex = 0;
    }

    activeEnemies = 0;
    spawnTimer = 0.0f;
}

void FindPath(Enemy* enemy, int startX, int startY, int goalX, int goalY) {
    findPathDFS(startX, startY, goalX, goalY, enemy->path, &enemy->pathLength);
}

void SpawnEnemy(void) {
    if (activeEnemies >= MAX_ENEMIES) return;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].active = true;

            enemies[i].gridX = 0;
            enemies[i].gridY = 0;
            enemies[i].x = CELL_SIZE / 2;
            enemies[i].y = CELL_SIZE / 2;

            FindPath(&enemies[i], 0, 0, COLS-1, ROWS-1);

            if (enemies[i].pathLength == 0) {
                enemies[i].active = false;
                return;
            }

            enemies[i].currentPathIndex = 0;
            activeEnemies++;
            break;
        }
    }
}

void UpdateEnemies(void) {
    float deltaTime = GetFrameTime();

    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        SpawnEnemy();
        spawnTimer = 0.0f;
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if (enemies[i].currentPathIndex < enemies[i].pathLength) {
                Vector2 targetPos = enemies[i].path[enemies[i].currentPathIndex];

                float dx = targetPos.x - enemies[i].x;
                float dy = targetPos.y - enemies[i].y;

                float distance = sqrtf(dx*dx + dy*dy);

                if (distance <= enemies[i].speed * deltaTime) {
                    enemies[i].x = targetPos.x;
                    enemies[i].y = targetPos.y;
                    enemies[i].currentPathIndex++;

                    enemies[i].gridX = (int)(enemies[i].x / CELL_SIZE);
                    enemies[i].gridY = (int)(enemies[i].y / CELL_SIZE);
                } else {
                    float moveX = (dx / distance) * enemies[i].speed * deltaTime;
                    float moveY = (dy / distance) * enemies[i].speed * deltaTime;

                    enemies[i].x += moveX;
                    enemies[i].y += moveY;

                    enemies[i].gridX = (int)(enemies[i].x / CELL_SIZE);
                    enemies[i].gridY = (int)(enemies[i].y / CELL_SIZE);
                }
            } else {
                enemies[i].active = false;
                activeEnemies--;

                playerLives--;
                if (playerLives <= 0) {
                    CloseWindow();
                }
            }
        }
    }
}

void DrawEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            DrawCircle(enemies[i].x, enemies[i].y, CELL_SIZE/3, BLUE);

            float healthBarWidth = CELL_SIZE * 0.8f;
            float healthBarHeight = 5.0f;
            float healthPercentage = (float)enemies[i].health / 100.0f;

            DrawRectangle(enemies[i].x - healthBarWidth/2, enemies[i].y - CELL_SIZE/2 - 10, healthBarWidth, healthBarHeight, RED);

            DrawRectangle(enemies[i].x - healthBarWidth/2, enemies[i].y - CELL_SIZE/2 - 10, healthBarWidth * healthPercentage, healthBarHeight, GREEN);
        }
    }

    DrawText(TextFormat("Lives: %d", playerLives), 10, 10, 20, WHITE);
    DrawText(TextFormat("Score: %d", playerScore), 10, 40, 20, WHITE);
}

void SpawnEnemies(void) {
    static bool initialized = false;
    if (!initialized) {
        InitEnemies();
        initialized = true;
    }

    UpdateEnemies();
    DrawEnemies();
}