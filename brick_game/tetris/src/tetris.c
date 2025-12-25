#include <stdbool.h>
#include <stdlib.h>

#include "fsm.h"
#include "tetris_lib.h"
// #include "tetris_internal.h"
#include "tetromino.h"

static int** AllocMatrix(int row, int cal);
static bool InitializeGameState(CoreGameState_t *game);
static GameInfo_t CoreToPublic(CoreGameState_t *core);

static UserInput_t user_input = {NOSIG, false, false};

void userInput(UserAction_t action, bool hold) {
  user_input.action = (Signals_t) action;
  user_input.hold = hold;
  user_input.fresh = true;
}

GameInfo_t updateCurrentState() {
  static CoreGameState_t game;
  
  static bool initialized = false;
  if (!initialized) 
    initialized = InitializeGameState(&game);

  SigAct(&game);

  return CoreToPublic(&game);
}

// ========================================================
// Auxiliary Functions
// ========================================================
static int** AllocMatrix(int row, int cal) {
  int** entery = malloc(row * cal * sizeof(int) + row * sizeof(int*));

  int* ptr = (int*)(entery + row);

  for (int y = 0; y < row; ++y) entery[y] = ptr + cal * y;

  for (int y = 0; y < row; ++y)
    for (int x = 0; x < cal; ++x) entery[y][x] = 0;

  return entery;
}

static bool InitializeGameState(CoreGameState_t *game) {
  game->field = AllocMatrix(20, 10);
  game->next = AllocMatrix(4, 4);

  game->usr_input = &user_input;

  #if 1 // TODO: add initialization for high_score
  FILE* file = fopen("./tetris_data.bin", "r");
  if (file) {
    fscanf(file, "%d", &(game->info.high_score));
    fclose(file);
  }
  #endif

  return true;
}

static GameInfo_t CoreToPublic(CoreGameState_t *core) {
  GameInfo_t public = {
    .field = core->field,
    .next = core->next,
    .score = core->info.score,
    .high_score = core->info.high_score,
    .level = core->info.level,
    .pause = core->info.pause,
    .speed = core->info.speed
  };

  return public;
}