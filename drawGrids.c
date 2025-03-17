//
// Created by theodor on 2025-03-17.
//
#include "screenData.h"
#include "raylib.h"

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
                DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, WHITE);
            }
        }
    }
}

void HandleWallPlacement() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();

        int cellX = getLeMousePosition.x / CELL_SIZE;
        int cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            grid[cellY][cellX] = CELL_WALL;
        }
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 getLeMousePosition = GetMousePosition();
        int cellX = getLeMousePosition.x / CELL_SIZE;
        int cellY = getLeMousePosition.y / CELL_SIZE;

        if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
            grid[cellY][cellX] = CELL_EMPTY;
        }
    }
}

void SpawnEnemies() {

}

void CreateTurret() {

}