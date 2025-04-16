#include "screenData.h"
#include "Raylib.h"
#include "simple_profiler.h"
#include <math.h>


Turret turrets[MAX_TURRETS];
int activeTurrets = 0;
Projectile projectiles[MAX_PROJECTILES];
int activeProjectiles = 0;

void InitTurrets(void) {
    PROFILE_START(InitTurrets);
    for (int i = 0; i < MAX_TURRETS; i++) {
        turrets[i].active = false;
        turrets[i].range = 150.0f;
        turrets[i].damage = 25.0f;
        turrets[i].fireRate = 1.0f;
        turrets[i].fireTimer = 0.0f;
        turrets[i].targetEnemyIndex = -1;
    }
    activeTurrets = 0;

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == CELL_TURRET) {
                for (int i = 0; i < MAX_TURRETS; i++) {
                    if (!turrets[i].active) {
                        turrets[i].active = true;
                        turrets[i].gridX = x;
                        turrets[i].gridY = y;
                        turrets[i].x = x * CELL_SIZE + CELL_SIZE / 2;
                        turrets[i].y = y * CELL_SIZE + CELL_SIZE / 2;
                        activeTurrets++;
                        break;
                    }
                }
            }
        }
    }
    PROFILE_END(InitTurrets);
}

void UpdateTurrets(void) {
    PROFILE_START(UpdateTurrets);
    float deltaTime = GetFrameTime();

    for (int i = 0; i < MAX_TURRETS; i++) {
        if (!turrets[i].active) continue;

        turrets[i].fireTimer -= deltaTime;

        float closestDistance = turrets[i].range;
        turrets[i].targetEnemyIndex = -1;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;

            float dx = turrets[i].x - enemies[j].x;
            float dy = turrets[i].y - enemies[j].y;
            float distance = sqrtf(dx*dx + dy*dy);

            if (distance <= closestDistance) {
                closestDistance = distance;
                turrets[i].targetEnemyIndex = j;
            }
        }

        if (turrets[i].targetEnemyIndex >= 0 && turrets[i].fireTimer <= 0) {
            FireProjectile(i, turrets[i].targetEnemyIndex);
            turrets[i].fireTimer = 1.0f / turrets[i].fireRate;
        }
    }

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x] == CELL_TURRET) {
                bool turretExists = false;
                for (int i = 0; i < MAX_TURRETS; i++) {
                    if (turrets[i].active && turrets[i].gridX == x && turrets[i].gridY == y) {
                        turretExists = true;
                        break;
                    }
                }

                if (!turretExists) {
                    for (int i = 0; i < MAX_TURRETS; i++) {
                        if (!turrets[i].active) {
                            turrets[i].active = true;
                            turrets[i].gridX = x;
                            turrets[i].gridY = y;
                            turrets[i].x = x * CELL_SIZE + CELL_SIZE / 2;
                            turrets[i].y = y * CELL_SIZE + CELL_SIZE / 2;
                            activeTurrets++;
                            break;
                        }
                    }
                }
            }
        }
    }
    PROFILE_END(UpdateTurrets);
}

void FireProjectile(int turretIndex, int enemyIndex) {

    PROFILE_START(FireProjectile);

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].active = true;
            projectiles[i].x = turrets[turretIndex].x;
            projectiles[i].y = turrets[turretIndex].y;
            projectiles[i].speed = 300.0f;
            projectiles[i].damage = turrets[turretIndex].damage;
            projectiles[i].targetEnemyIndex = enemyIndex;
            activeProjectiles++;
            return;
        }
    }
    PROFILE_END(FireProjectile);
}

void UpdateProjectiles(void) {
    PROFILE_START(UpdateProjectiles);

    float deltaTime = GetFrameTime();

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;

        if (projectiles[i].targetEnemyIndex >= 0 &&
            projectiles[i].targetEnemyIndex < MAX_ENEMIES &&
            enemies[projectiles[i].targetEnemyIndex].active) {

            float targetX = enemies[projectiles[i].targetEnemyIndex].x;
            float targetY = enemies[projectiles[i].targetEnemyIndex].y;

            float dx = targetX - projectiles[i].x;
            float dy = targetY - projectiles[i].y;
            float distance = sqrtf(dx*dx + dy*dy);

            if (distance <= 10.0f) {

                enemies[projectiles[i].targetEnemyIndex].health -= projectiles[i].damage;

                if (enemies[projectiles[i].targetEnemyIndex].health <= 0) {
                    enemies[projectiles[i].targetEnemyIndex].active = false;
                    activeEnemies--;
                    playerScore += 10;
                }

                projectiles[i].active = false;
                activeProjectiles--;
            } else {
                float moveX = (dx / distance) * projectiles[i].speed * deltaTime;
                float moveY = (dy / distance) * projectiles[i].speed * deltaTime;

                projectiles[i].x += moveX;
                projectiles[i].y += moveY;
            }
        } else {
            projectiles[i].active = false;
            activeProjectiles--;
        }

        if (projectiles[i].x < 0 || projectiles[i].x > SCREEN_WIDTH ||
            projectiles[i].y < 0 || projectiles[i].y > SCREEN_HEIGHT) {
            projectiles[i].active = false;
            activeProjectiles--;
        }
    }
    PROFILE_END(UpdateProjectiles);
}

void DrawProjectiles(void) {
    PROFILE_START(DrawProjectiles);
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            DrawCircle(projectiles[i].x, projectiles[i].y, 5, YELLOW);
        }
    }
    PROFILE_END(DrawProjectiles);
}

void DrawTurrets(void) {

    PROFILE_START(DrawTurrets);

    for (int i = 0; i < MAX_TURRETS; i++) {
        if (turrets[i].active) {

            DrawRectangle(turrets[i].gridX * CELL_SIZE + 1, turrets[i].gridY * CELL_SIZE + 1,
                         CELL_SIZE - 2, CELL_SIZE - 2, RED);

            DrawRectangle(turrets[i].gridX * CELL_SIZE + CELL_SIZE/4,
                         turrets[i].gridY * CELL_SIZE + CELL_SIZE/4,
                         CELL_SIZE/2, CELL_SIZE/2, WHITE);

            if (turrets[i].targetEnemyIndex >= 0 &&
                turrets[i].targetEnemyIndex < MAX_ENEMIES &&
                enemies[turrets[i].targetEnemyIndex].active) {

                float targetX = enemies[turrets[i].targetEnemyIndex].x;
                float targetY = enemies[turrets[i].targetEnemyIndex].y;

                DrawLineEx((Vector2){turrets[i].x, turrets[i].y},
                          (Vector2){turrets[i].x + (targetX - turrets[i].x) * 0.5f,
                                   turrets[i].y + (targetY - turrets[i].y) * 0.5f},
                          3.0f, YELLOW);
                }

        }
    }
    PROFILE_END(DrawTurrets);
}

void InitProjectiles(void) {
    PROFILE_START(InitProjectiles);
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }
    activeProjectiles = 0;
    PROFILE_END(InitProjectiles);
}
