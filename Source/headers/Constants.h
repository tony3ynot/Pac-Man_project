#pragma once

#define WINDOW_X 300
#define WINDOW_Y 0

#define WINDOW_W 580        // window's width
#define WINDOW_H 800        // window's height

#define BOUNDARY_X (WINDOW_W)/2
#define BOUNDARY_Y (WINDOW_H)/2

#define NUM_COL 29        // map grid width
#define NUM_ROW 31        // map grid height

#define BLOCK_SIZE 20
#define MOVE_SPEED 1

#define LEFT_BOUNDARY (-(NUM_COL / 2) * BLOCK_SIZE)
#define RIGHT_BOUNDARY ((NUM_COL / 2) * BLOCK_SIZE)
#define TOP_BOUNDARY ((NUM_ROW / 2) * BLOCK_SIZE)
#define BOTTOM_BOUNDARY (-(NUM_ROW / 2) * BLOCK_SIZE)
