#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screenData.h"

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib tower defence thing");
    SetTargetFPS(120);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        GridDraw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}