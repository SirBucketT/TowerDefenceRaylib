//
// Created by theodor on 2025-03-17.
//
#include "main.c"
#include "screenData.h"
#include "raylib.h"

DrawGrid() {

    for (int i = 0; i <= COLS; i++) {
        DrawLine(
            i * CELL_SIZE,          // startPosX
            0,                      // startPosY
            i * CELL_SIZE,          // endPosX
            SCREEN_HEIGHT,          // endPosY
            DARKGRAY                // color
        );
    }

    for (int i = 0; i <= ROWS; i++) {
        DrawLine(
            0,                      // startPosX
            i * CELL_SIZE,          // startPosY
            SCREEN_WIDTH,           // endPosX
            i * CELL_SIZE,          // endPosY
            DARKGRAY                // color
        );
    }
}