#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screenData.h"
#include "drawGrids.c"

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib tower defence thing");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawGrid();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}