#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screenData.h"
#include "drawGrids.c"

int gameState;

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib tower defence thing");
    SetTargetFPS(120);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyDown(KEY_SPACE)) {
            if (gameState == 0) {
                gameState++;
            } else {
                gameState--;
            }
        }

        switch (gameState) {
            case 0:
                GridDraw();
                DrawWalls();
                HandleWallPlacement();
            case 1:
                void spawnEnemies();
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}