#include "screenData.h"
#include "raylib.h"
#include <math.h>

#include "turretManager.c"

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
int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

Vector2 previewPath[ROWS * COLS];
int previewPathLength = 0;

bool isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

//Performance stuff
//FILE *MainTest = fopen("name.txt", "w");
//float deltaTime = GetFrameTime();
//int fps = GetFPS();
//fprintf(MainTest, "Frame Time Main Function: %f ms | FPS: %d\n", deltaTime * 1000, fps);
//fclose(MainTest);


void DrawWalls() {
    FILE *TestDrawWall = fopen("TestDrawWalls.txt", "w");
    float deltaTime = GetFrameTime();
    int fps = GetFPS();
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == CELL_WALL) {
                DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, GRAY);
            }
        }
    }
    fprintf(TestDrawWall, "Frame Time GridWalls Function: %f ms | FPS: %d\n", deltaTime * 1000, fps);
    fclose(TestDrawWall);
}

void HandleWallPlacement() {
    FILE *tester = fopen("TestHandleWallPlacement.txt", "w");
    float deltaTime = GetFrameTime();
    int fps = GetFPS();
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
    fprintf(tester, "Frame Time for HandleWallPlacement Function: %f ms | FPS: %d\n", deltaTime * 1000, fps);
    fclose(tester);
}

void CreateTurret() {
    FILE *tester = fopen("TestCreateTurret.txt", "w");
    float deltaTime = GetFrameTime();
    int fps = GetFPS();
    static bool initialized = false;
    if (!initialized) {
        InitTurrets();
        InitProjectiles();
        initialized = true;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();

        cellX = getLeMousePosition.x / CELL_SIZE;
        cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            // Allow placing turrets on any non-wall cell
            if (grid[cellY][cellX] != CELL_WALL && grid[cellY][cellX] != CELL_TURRET) {
                grid[cellY][cellX] = CELL_TURRET;

                // Initialize a new turret at this position
                for (int i = 0; i < MAX_TURRETS; i++) {
                    if (!turrets[i].active) {
                        turrets[i].active = true;
                        turrets[i].gridX = cellX;
                        turrets[i].gridY = cellY;
                        turrets[i].x = cellX * CELL_SIZE + CELL_SIZE / 2;
                        turrets[i].y = cellY * CELL_SIZE + CELL_SIZE / 2;
                        activeTurrets++;
                        break;
                    }
                }
            }
        }
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();
        cellX = getLeMousePosition.x / CELL_SIZE;
        cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            if (grid[cellY][cellX] == CELL_TURRET) {
                // When removing a turret, change the cell back to CELL_EMPTY or another appropriate terrain
                grid[cellY][cellX] = CELL_GRASS; // Or CELL_EMPTY if you prefer

                // Deactivate any turret at this position
                for (int i = 0; i < MAX_TURRETS; i++) {
                    if (turrets[i].active && turrets[i].gridX == cellX && turrets[i].gridY == cellY) {
                        turrets[i].active = false;
                        activeTurrets--;
                        break;
                    }
                }
            }
        }
    }
    fprintf(tester, "Frame Time for creating turrets: %f ms | FPS: %d\n", deltaTime * 1000, fps);
    fclose(tester);
}

void initPathfindingGrid() {
    FILE *tester = fopen("TestInitPathFindingGrid.txt", "w");
    float deltaTime = GetFrameTime();
    int fps = GetFPS();
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            pathNodes[y][x].x = x;
            pathNodes[y][x].y = y;
            pathNodes[y][x].visited = false;
            pathNodes[y][x].parent = NULL;

            pathNodes[y][x].obstacle = (grid[y][x] == CELL_WALL || grid[y][x] == CELL_TURRET);
        }
    }
    fprintf(tester, "Frame Time for Initializing path finding grid: %f ms | FPS: %d\n", deltaTime * 1000, fps);
    fclose(tester);
}

bool findPathBFS(int startX, int startY, int goalX, int goalY, Vector2 path[], int* pathLength) {
    initPathfindingGrid();

    Queue queue;

    initQueue(&queue);

    Node* startNode = &pathNodes[startY][startX];
    startNode->visited = true;
    enqueue(&queue, startNode);

    bool foundPath = false;

    while (!isQueueEmpty(&queue)) {
        Node* current = dequeue(&queue);

        if (current->x == goalX && current->y == goalY) {
            foundPath = true;
            break;
        }

        for (int i = 0; i < 8; i++) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS) {
                Node* neighbor = &pathNodes[newY][newX];


                if (i == 1 || i == 3 || i == 5 || i == 7) {

                    bool blockingWall = false;

                    if (i == 1) {
                        blockingWall = pathNodes[current->y][current->x + 1].obstacle ||
                                      pathNodes[current->y + 1][current->x].obstacle;
                    } else if (i == 3) {
                        blockingWall = pathNodes[current->y][current->x - 1].obstacle ||
                                      pathNodes[current->y + 1][current->x].obstacle;
                    } else if (i == 5) {
                        blockingWall = pathNodes[current->y][current->x - 1].obstacle ||
                                      pathNodes[current->y - 1][current->x].obstacle;
                    } else if (i == 7) {
                        blockingWall = pathNodes[current->y][current->x + 1].obstacle ||
                                      pathNodes[current->y - 1][current->x].obstacle;
                    }

                    if (blockingWall) {
                        continue;
                    }
                }

                if (!neighbor->visited && !neighbor->obstacle) {
                    neighbor->visited = true;
                    neighbor->parent = current;
                    enqueue(&queue, neighbor);
                }
            }
        }
    }

    if (foundPath) {
        Node* current = &pathNodes[goalY][goalX];
        int idx = 0;
        Vector2 tempPath[ROWS * COLS];

        while (current != NULL) {
            tempPath[idx++] = (Vector2){ current->x * CELL_SIZE + CELL_SIZE / 2, current->y * CELL_SIZE + CELL_SIZE / 2 };
            current = current->parent;
        }

        for (int i = 0; i < idx; i++) {
            path[i] = tempPath[idx - 1 - i];
        }

        *pathLength = idx;
        return true;
    }

    *pathLength = 0;
    return false;
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
            ApplyTerrainEffects(&enemies[i], deltaTime);

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


void InitEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].health = 100;
        enemies[i].speed = 100.0f;
        enemies[i].baseSpeed = 100.0f;
        enemies[i].pathLength = 0;
        enemies[i].currentPathIndex = 0;
        enemies[i].isWet = false;
        enemies[i].wetTimer = 0.0f;
    }

    activeEnemies = 0;
    spawnTimer = 0.0f;
}


void FindPath(Enemy* enemy, int startX, int startY, int goalX, int goalY) {
    findPathBFS(startX, startY, goalX, goalY, enemy->path, &enemy->pathLength);
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

void DrawEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            Color enemyColor = BLUE;

            if (enemies[i].isWet) {
                enemyColor = DARKBLUE;

                for (int d = 0; d < 3; d++) {
                    float offsetX = (rand() % 20) - 10;
                    float offsetY = (rand() % 20) - 10;
                    DrawCircle(enemies[i].x + offsetX, enemies[i].y + offsetY, 2, SKYBLUE);
                }
            }

            DrawCircle(enemies[i].x, enemies[i].y, CELL_SIZE/3, enemyColor);

            float healthBarWidth = CELL_SIZE * 0.8f;
            float healthBarHeight = 5.0f;
            float healthPercentage = (float)enemies[i].health / 100.0f;

            DrawRectangle(enemies[i].x - healthBarWidth/2, enemies[i].y - CELL_SIZE/2 - 10,
                         healthBarWidth, healthBarHeight, RED);

            DrawRectangle(enemies[i].x - healthBarWidth/2, enemies[i].y - CELL_SIZE/2 - 10,
                         healthBarWidth * healthPercentage, healthBarHeight, GREEN);
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

void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

bool isQueueEmpty(Queue* q) {
    return q->front == q->rear;
}

void enqueue(Queue* q, Node* node) {
    q->nodes[q->rear++] = node;
}

Node* dequeue(Queue* q) {
    return q->nodes[q->front++];
}

void DrawPreviewPath(void) {
    for (int i = 0; i < previewPathLength - 1; i++) {
        Vector2 start = previewPath[i];
        Vector2 end = previewPath[i + 1];
        DrawLineEx(start, end, 4.0f, YELLOW);
    }
}