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