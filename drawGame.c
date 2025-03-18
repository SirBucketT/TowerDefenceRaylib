#include "screenData.h"
#include "raylib.h"

CellType grid[ROWS][COLS] = {0};
int cellX;
int cellY;

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
    // Initialize the pathfinding grid
    initPathfindingGrid();

    // Create the stack for DFS
    Stack stack;
    initStack(&stack);

    // Push the start node onto the stack
    pushStack(&stack, &pathNodes[startY][startX]);

    // Mark the start node as visited
    pathNodes[startY][startX].visited = true;

    // Main DFS loop
    while (!isStackEmpty(&stack) && !foundPath) {
        // Get the current node from the top of the stack
        Node* currentNode = popStack(&stack);

        // Check if we've reached the goal
        if (currentNode->x == goalX && currentNode->y == goalY) {
            foundPath = true;
            break;
        }

        // Explore neighbors (in our defined order)
        for (int i = 0; i < 4; i++) {
            // Calculate new position
            int newX = currentNode->x + dx[i];
            int newY = currentNode->y + dy[i];

            // Check if the new position is valid
            if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS) {
                // Check if the node is not an obstacle and not visited
                if (!pathNodes[newY][newX].obstacle && !pathNodes[newY][newX].visited) {
                    // Mark as visited
                    pathNodes[newY][newX].visited = true;

                    // Set the parent for path reconstruction
                    pathNodes[newY][newX].parent = currentNode;

                    // Push to the stack
                    pushStack(&stack, &pathNodes[newY][newX]);
                }
            }
        }

        // If we found a path, reconstruct it
        if (foundPath) {
            // Start from the goal node
            Node* pathNode = &pathNodes[goalY][goalX];
            int pathIndex = 0;

            // Temporary array to store the path (in reverse order)
            Vector2 tempPath[ROWS * COLS];

            // Traverse backwards from goal to start
            while (pathNode != NULL) {
                // Store the position in the path
                tempPath[pathIndex].x = pathNode->x * CELL_SIZE + CELL_SIZE / 2;
                tempPath[pathIndex].y = pathNode->y * CELL_SIZE + CELL_SIZE / 2;
                pathIndex++;

                // Move to the parent node
                pathNode = pathNode->parent;
            }

            // Reverse the path (so it goes from start to goal)
            for (int i = 0; i < pathIndex; i++) {
                path[i] = tempPath[pathIndex - 1 - i];
            }

            void InitEnemies(void) {
                for (int i = 0; i < MAX_ENEMIES; i++) {
                    enemies[i].active = false;
                    enemies[i].health = 100;
                    enemies[i].speed = 100.0f; // Pixels per second
                    enemies[i].pathLength = 0;
                    enemies[i].currentPathIndex = 0;
                }

                activeEnemies = 0;
                spawnTimer = 0.0f;
            }

            void SpawnEnemy(void) {
                if (activeEnemies >= MAX_ENEMIES) return;

                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (!enemies[i].active) {
                        // Set the enemy as active
                        enemies[i].active = true;

                        // Set the starting position (top-left corner)
                        enemies[i].gridX = 0;
                        enemies[i].gridY = 0;
                        enemies[i].x = CELL_SIZE / 2;
                        enemies[i].y = CELL_SIZE / 2;

                        // Find a path to the goal (bottom-right corner)
                        FindPath(&enemies[i], 0, 0, COLS-1, ROWS-1);

                        void UpdateEnemies(void) {
                            float deltaTime = GetFrameTime();

                            // Update spawn timer
                            spawnTimer += deltaTime;

                            if (spawnTimer >= spawnInterval) {
                                SpawnEnemy();
                                spawnTimer = 0.0f;
                            }

                            for (int i = 0; i < MAX_ENEMIES; i++) {
                                if (enemies[i].active) {
                                    // Move the enemy along its path
                                    if (enemies[i].currentPathIndex < enemies[i].pathLength) {
                                        // Get the target position from the path
                                        Vector2 targetPos = enemies[i].path[enemies[i].currentPathIndex];

                                        // Calculate direction vector to the target
                                        float dx = targetPos.x - enemies[i].x;
                                        float dy = targetPos.y - enemies[i].y;

                                        // Calculate distance to the target
                                        float distance = sqrtf(dx*dx + dy*dy);

                                        void DrawEnemies(void) {
                                            for (int i = 0; i < MAX_ENEMIES; i++) {
                                                if (enemies[i].active) {
                                                    // Draw the enemy as a blue circle
                                                    DrawCircle(enemies[i].x, enemies[i].y, CELL_SIZE/3, BLUE);

                                                    // Draw health bar
                                                    float healthBarWidth = CELL_SIZE * 0.8f;
                                                    float healthBarHeight = 5.0f;
                                                    float healthPercentage = (float)enemies[i].health / 100.0f;

                                                    void SpawnEnemies() {
                                                        UpdateEnemies();
                                                        DrawEnemies();
                                                    }



void SpawnEnemies() {

}
