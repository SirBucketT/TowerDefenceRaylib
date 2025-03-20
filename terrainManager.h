#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

#include "raylib.h"
#include "screenData.h"

void GenerateRandomTerrain(void);

void DrawTerrain(void);

void ApplyTerrainEffects(Enemy* enemy, float deltaTime);

#endif