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

        GridDraw();
        DrawWalls();

        //===========================================================
        //     Enter or exit edit wall mode
        //===========================================================

        if (IsKeyDown(KEY_SPACE)) {
            if (gameState == 0) {
                gameState++;
            } else if (gameState == 1) {
                gameState--;
            }
        }

        switch (gameState) {
            case 0:
                HandleWallPlacement();
                CreateTurret();
            case 1:
                SpawnEnemies();
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}