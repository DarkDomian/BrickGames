#include "tetromino.h"

static void FisherYatesShuffle(Tetromino_t *bag, int size);

const Tetromino kTetrShape[NUMBER_OF_TETROMINO] = {
    {SHAPE_I, {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 4, 1},
    {SHAPE_O, {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 2, 2},
    {SHAPE_T, {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 3},
    {SHAPE_S, {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 4},
    {SHAPE_Z, {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 5},
    {SHAPE_J, {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 6},
    {SHAPE_L, {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 7},
};

Tetromino_t GetNextPiece() {
  static Tetromino_t random_bag[NUMBER_OF_TETROMINO];
  static int bag_index = NUMBER_OF_TETROMINO;

  if (bag_index >= NUMBER_OF_TETROMINO) {
    for (int i = 0; i < NUMBER_OF_TETROMINO; i++) random_bag[i] = i;
    FisherYatesShuffle(random_bag, NUMBER_OF_TETROMINO);
    bag_index = 0;
  }

  return random_bag[bag_index++];
}

// Fisher–Yates shuffle Algorithm
static void FisherYatesShuffle(Tetromino_t *bag, int size) {
  static bool initialize = false;
  if (!initialize) {
    srand(time(NULL));
    initialize = true;
  }

  for (int i = size - 1; i > 0; --i) {
    int j = rand() % (i + 1);

    int temp = bag[i];
    bag[i] = bag[j];
    bag[j] = temp;
  }
}