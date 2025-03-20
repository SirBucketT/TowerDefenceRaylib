#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screenData.h"
#include "drawGame.c"
#include "terrainManager.c"  // Include our new terrain manager

int gameState = 0;

int main(void) {
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib tower defence thing");
    SetTargetFPS(120);

    GenerateRandomTerrain();
    DrawTerrain();

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        //=========================================================================================================================
        //     Game states:
        //     0: Wall placement phase
        //     1: Turret placement phase
        //     2: Game running phase
        //=========================================================================================================================

        if (IsKeyPressed(KEY_SPACE)) {
            if (gameState == 0) {
                gameState++;
            } else if (gameState == 1) {
                gameState++;
            }
        }

        if ((gameState == 0 || gameState == 1) && IsKeyPressed(KEY_R)) {
            GenerateRandomTerrain();
        }

        if (gameState == 0 || gameState == 1) {
            findPathBFS(0, 0, COLS - 1, ROWS - 1, previewPath, &previewPathLength);
        }



        switch (gameState) {
            case 0:
                HandleWallPlacement();

            DrawText("WALL PLACEMENT PHASE", SCREEN_WIDTH/2 - 150, 10, 20, WHITE);
            DrawText("Left click: Add wall | Right click: Remove wall", 10, SCREEN_HEIGHT - 30, 15, WHITE);
            DrawText("Press SPACE to continue | Press R to regenerate terrain", 10, SCREEN_HEIGHT - 50, 15, WHITE);

            break;
            case 1:
                CreateTurret();
            DrawText("TURRET PLACEMENT PHASE", SCREEN_WIDTH/2 - 150, 10, 20, WHITE);
            DrawText("Left click: Add turret | Right click: Remove turret", 10, SCREEN_HEIGHT - 30, 15, WHITE);
            DrawText("Press SPACE to start game | Press R to regenerate terrain", 10, SCREEN_HEIGHT - 50, 15, WHITE);
            break;
            case 2:
                SpawnEnemies();
            UpdateTurrets();
            UpdateProjectiles();
            break;
        }

        GridDraw();
        DrawWalls();
        DrawTurrets();
        DrawPreviewPath();

        if (gameState == 2) {
            DrawProjectiles();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}