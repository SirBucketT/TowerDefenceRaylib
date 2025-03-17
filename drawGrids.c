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
        DrawLine(0, i * CELL_SIZE,SCREEN_WIDTH, i * CELL_SIZE, WHITE);
    }
}

void DrawWalls() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == CELL_WALL) {
                // Draw a filled rectangle for wall cells
                DrawRectangle(
                    x * CELL_SIZE + 1,      // x position (offset by 1 to avoid covering grid lines)
                    y * CELL_SIZE + 1,      // y position
                    CELL_SIZE - 2,          // width (reduced by 2 to avoid covering grid lines)
                    CELL_SIZE - 2,          // height
                    DARKGRAY                // wall color
                );
            }
        }
    }
}

