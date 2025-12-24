#ifndef FSM_H
#define FSM_H

#include <stdio.h>

#include "tetris_lib.h"
#include "tetromino.h"

typedef enum {
  START = 0,
  PAUSE,
  GAME_OVER
} TetrisState_t;

typedef enum {
  START_BTN,
  PAUSE_BTN,
  TERM_BTN,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,
  ACTION_BTN,
  NOSIG
} Signals_t;

// public method of FSM
// signals get_signal(int user_input);
void SigAct(Signals_t sig, GameInfo_t *snapshot, TetrisState_t *state);

#endif // FSM_H