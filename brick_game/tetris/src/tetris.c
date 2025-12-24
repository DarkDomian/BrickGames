#include <stdbool.h>
#include <stdlib.h>

#include "tetris_lib.h"
#include "tetromino.h"
#include "fsm.h"

typedef struct {
  Signals_t action;
  bool hold;
  bool fresh;
} UserInput_t;

static int** AllocMatrix(int row, int cal);

// "gloval" variable
static GameInfo_t game_snapshot;
static UserInput_t usr_input;


// possible to translate from UserAction_t to Signals (for example)
void userInput(UserAction_t action, bool hold) {
  usr_input.action = (Signals_t)action;
  usr_input.hold = hold;
  usr_input.fresh = true;
}

GameInfo_t updateCurrentState() {
  static bool first_call = true;
  static TetrisState_t current_state = START;

  // initialize on first call
  if (first_call) {
    game_snapshot.field = AllocMatrix(20, 10);
    game_snapshot.next = AllocMatrix(4, 4);
    first_call = false;
  }

  SigAct(usr_input.fresh ? usr_input.action : NOSIG, &game_snapshot, &current_state);
  usr_input.fresh = false;

  return game_snapshot;
}


// other functions
static int** AllocMatrix(int row, int cal) {
  int** entery = malloc(row * cal * sizeof(int) + row * sizeof(int*));

  int* ptr = (int*)(entery + row);

  for (int y = 0; y < row; ++y) entery[y] = ptr + cal * y;

  for (int y = 0; y < row; ++y)
    for (int x = 0; x < cal; ++x) entery[y][x] = 0;

  return entery;
}