#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_TETROMINO 7

typedef enum {
  SHAPE_I,
  SHAPE_O,
  SHAPE_T,
  SHAPE_S,
  SHAPE_Z,
  SHAPE_J,
  SHAPE_L
} Tetromino_t;

typedef struct {
  Tetromino_t type;
  int matrix[4][4];
  int size;
  int color;
} Tetromino;

extern const Tetromino SHAPES[NUMBER_OF_TETROMINO];

Tetromino_t GetNextPiece(void);

#endif  // TETROMINO_H