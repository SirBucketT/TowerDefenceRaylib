#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screenData.h"
#include "drawGame.c"
#include "terrainManager.c"
#include "simple_profiler.h"

//=========================================================================================================================
//     Game states:
//     3: Main Menu
//     0: Wall placement phase
//     1: Turret placement phase
//     2: Game running phase
//=========================================================================================================================
int gameState = 3;

// Define menu button areas
Rectangle playButton = { 0 };
Rectangle quitButton = { 0 };

void DrawMainMenu(void) {
    FILE *MenuTest = fopen("TestMenu.txt", "w");
    float deltaTime = GetFrameTime();
    int fps = GetFPS();
    PROFILE_START(DrawMainMenu);

    DrawText("TOWER DEFENCE GAME OR SOMETHING", SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/4, 30, WHITE);

    playButton = (Rectangle){ SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 50 };
    quitButton = (Rectangle){ SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 80, 200, 50 };

    DrawRectangleRec(playButton, DARKGRAY);
    DrawRectangleLinesEx(playButton, 2, WHITE);
    DrawText("PLAY GAME", playButton.x + 40, playButton.y + 15, 20, WHITE);

    DrawRectangleRec(quitButton, DARKGRAY);
    DrawRectangleLinesEx(quitButton, 2, WHITE);
    DrawText("QUIT GAME", quitButton.x + 40, quitButton.y + 15, 20, WHITE);

    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, playButton)) {
        DrawRectangleRec(playButton, GRAY);
        DrawRectangleLinesEx(playButton, 2, WHITE);
        DrawText("PLAY GAME", playButton.x + 40, playButton.y + 15, 20, WHITE);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            gameState = 0;
            GenerateRandomTerrain();
        }
    }

    if (CheckCollisionPointRec(mousePoint, quitButton)) {
        DrawRectangleRec(quitButton, GRAY);
        DrawRectangleLinesEx(quitButton, 2, WHITE);
        DrawText("QUIT GAME", quitButton.x + 40, quitButton.y + 15, 20, WHITE);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
        }
    }
    PROFILE_END(DrawMainMenu);
    fprintf(MenuTest, "Frame Time Main Function: %f ms | FPS: %d\n", deltaTime * 1000, fps);
    fclose(MenuTest);
}

int main(void) {
    PROFILE_START(main);
    FILE *MainTest = fopen("TestMain.txt", "w");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib tower defence thing");
    //SetTargetFPS(120);

    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        int fps = GetFPS();
        BeginDrawing();
        ClearBackground(BLACK);
        if (gameState >= 0 && IsKeyPressed(KEY_SPACE)) {
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
                DrawTerrain();
                DrawPreviewPath();
                HandleWallPlacement();
                DrawText("WALL PLACEMENT PHASE", SCREEN_WIDTH/2 - 150, 10, 20, WHITE);
                DrawText("Left click: Add wall | Right click: Remove wall", 10, SCREEN_HEIGHT - 30, 15, WHITE);
                DrawText("Press SPACE to continue | Press R to regenerate terrain", 10, SCREEN_HEIGHT - 50, 15, WHITE);
                DrawWalls();
                break;

            case 1:
                DrawTerrain();
                DrawPreviewPath();
                CreateTurret();
                DrawText("TURRET PLACEMENT PHASE", SCREEN_WIDTH/2 - 150, 10, 20, WHITE);
                DrawText("Left click: Add turret | Right click: Remove turret", 10, SCREEN_HEIGHT - 30, 15, WHITE);
                DrawText("Press SPACE to start game | Press R to regenerate terrain", 10, SCREEN_HEIGHT - 50, 15, WHITE);
                DrawText("Press ESC to return to menu", 10, SCREEN_HEIGHT - 70, 15, WHITE);
                DrawWalls();
                DrawTurrets();
                break;

            case 2:
                DrawTerrain();
                SpawnEnemies();
                UpdateTurrets();
                UpdateProjectiles();
                DrawWalls();
                DrawTurrets();
                DrawProjectiles();
                break;
            case 3:
                DrawMainMenu();
            break;
        }
        fprintf(MainTest, "Frame Time Main Function: %f ms | FPS: %d\n", deltaTime * 1000, fps);
        EndDrawing();
    }
    fclose(MainTest);
    PROFILE_END(main);
    CloseWindow();
    ReportProfilingResults();
    return 0;
}