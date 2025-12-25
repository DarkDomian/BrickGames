#ifndef FSM_H
#define FSM_H

#include <stdio.h>

#include "tetris_lib.h"
#include "tetris_internal.h"
#include "tetromino.h"

typedef enum { START = 0, PAUSE, GAME_OVER } TetrisState_t;

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


typedef struct {
  Signals_t action;
  bool hold;
  bool fresh;
} UserInput_t;

typedef struct {
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} InfoBar_t;

typedef struct {
  Tetromino_t current_tetr;
  Tetromino_t next_tetr;
  int pos_y;
  int pos_x;
  int rotation;
} TetrominoInfo_t;

typedef struct {
  TetrisState_t state;
  TetrominoInfo_t tetr;
  UserInput_t *usr_input;
  InfoBar_t info;
  int **field;
  int **next;
} CoreGameState_t;


// public method of FSM
// signals get_signal(int user_input);
void SigAct(CoreGameState_t *game);

#endif  // FSM_H