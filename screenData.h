//
// Created by theodor on 2025-03-13.
//

#ifndef SCREENDATA_H
#define SCREENDATA_H

#define CELL_SIZE       40

#define SCREEN_WIDTH    799
#define SCREEN_HEIGHT   800

#define ROWS            20
#define COLS            20

typedef enum {
    CELL_EMPTY = 0,
    CELL_WALL
} CellType;

void GridDraw(void);
void DrawWalls(void);
void HandleWallPlacement(void);

#endif //SCREENDATA_H
