#include "screenData.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

TerrainProperties terrainProps[6] = {
    {1.0f, BLACK, false, 0.0f},
    {0.0f, GRAY, false, 0.0f},
    {1.0f, RED, false, 0.0f},
    {0.5f, SKYBLUE, true, 3.0f},
    {0.8f, BROWN, false, 0.0f},
    {1.0f, GREEN, false, 0.0f}
};

void GenerateRandomTerrain(void) {
    srand(time(NULL));

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if ((x < 2 && y < 2) || (x > COLS-3 && y > ROWS-3)) {
                grid[y][x] = CELL_EMPTY;
                continue;
            }

            int randVal = rand() % 100;

            if (randVal < 60) {
                grid[y][x] = CELL_GRASS;
            } else if (randVal < 80) {
                grid[y][x] = CELL_DIRT;
            } else {
                grid[y][x] = CELL_WATER;
            }
        }
    }

    Vector2 testPath[ROWS * COLS];
    int pathLength = 0;

    bool pathExists = findPathBFS(0, 0, COLS-1, ROWS-1, testPath, &pathLength);

    while (!pathExists) {
        int x = rand() % COLS;
        int y = rand() % ROWS;

        if ((x < 2 && y < 2) || (x > COLS-3 && y > ROWS-3)) {
            continue;
        }

        if (grid[y][x] != CELL_EMPTY && grid[y][x] != CELL_WALL && grid[y][x] != CELL_TURRET) {
            grid[y][x] = CELL_EMPTY;
            pathExists = findPathBFS(0, 0, COLS-1, ROWS-1, testPath, &pathLength);
        }
    }
}

void DrawTerrain(void) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            CellType type = grid[y][x];

            if (type != CELL_WALL && type != CELL_TURRET && type != CELL_EMPTY) {
                DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1,
                             CELL_SIZE - 2, CELL_SIZE - 2,
                             terrainProps[type].color);
            }
        }
    }
}

// Apply terrain effects to an enemy
void ApplyTerrainEffects(Enemy* enemy, float deltaTime) {
    int cellX = enemy->gridX;
    int cellY = enemy->gridY;

    if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
        CellType terrainType = grid[cellY][cellX];

        float speedMultiplier = terrainProps[terrainType].speedMultiplier;

        if (terrainProps[terrainType].causesWetEffect) {
            enemy->isWet = true;
            enemy->wetTimer = terrainProps[terrainType].wetDuration;
        }

        // Additional slowdown if the enemy is wet and on dirt
        if (enemy->isWet && terrainType == CELL_DIRT) {
            speedMultiplier = 0.6f;
        }

        enemy->speed = enemy->baseSpeed * speedMultiplier;
    }

    if (enemy->isWet) {
        enemy->wetTimer -= deltaTime;

        if (enemy->wetTimer <= 0.0f) {
            enemy->isWet = false;

            if (grid[enemy->gridY][enemy->gridX] == CELL_EMPTY ||
                grid[enemy->gridY][enemy->gridX] == CELL_GRASS) {
                enemy->speed = enemy->baseSpeed;
            }
        }
    }
}