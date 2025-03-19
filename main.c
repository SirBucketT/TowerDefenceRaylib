#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screenData.h"
#include "drawGame.c"

int gameState = 0;

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib tower defence thing");
    SetTargetFPS(120);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        //=========================================================================================================================
        //     Game starts with player making walls then create turrets.
        //     Once the turrets and walls placement is final and the game starts and runs until player gets game over.
        //=========================================================================================================================

        if (IsKeyPressed(KEY_SPACE)) {
            if (gameState == 0) {
                gameState++;
            } else if (gameState == 1) {
                gameState++;
            }
        }

        switch (gameState) {
            case 0:
                HandleWallPlacement();
            break;
            case 1:
                CreateTurret();
            break;
            case 2:
                SpawnEnemies();
            break;
        }

        if (gameState == 0 || gameState == 1) {
            findPathBFS(0, 0, COLS - 1, ROWS - 1, previewPath, &previewPathLength);
        }

        GridDraw();
        DrawWalls();
        DrawTurret();
        DrawPreviewPath();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}