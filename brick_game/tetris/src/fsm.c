#include "fsm.h"

typedef struct Game_Params {
  int **field;
  int **next;
  int *score;
  int *high_score;
  int *level;
  int *pause;
  TetrisState_t *state;
} Params_t;

typedef void (*FunctionCallback_t)(Params_t *prms);

static void Spawn(Params_t *prms);
static void PauseGame(Params_t *prms);
static void RestartGame(Params_t *prms);
static void ExitGame(Params_t *prms);

FunctionCallback_t fsm_table[3][9] = {
    {Spawn, PauseGame, ExitGame, NULL, NULL, NULL, NULL, NULL, NULL},
    {PauseGame, NULL, ExitGame, PauseGame, PauseGame, PauseGame, PauseGame, PauseGame, NULL},
    {RestartGame, PauseGame, ExitGame, RestartGame, RestartGame, RestartGame, RestartGame, RestartGame, RestartGame}
};

void SigAct(Signals_t sig, GameInfo_t *snapshot, TetrisState_t *state) {
  FunctionCallback_t act = NULL;
  Params_t prms;

  prms.field = snapshot->field;
  prms.next = snapshot->next;
  prms.score = &snapshot->score;
  prms.high_score = &snapshot->high_score;
  prms.level = &snapshot->level;
  prms.pause = &snapshot->pause;
  prms.state = state;

  act = fsm_table[*state][sig];

  if (act) act(&prms);
}

static void Spawn(Params_t *prms) {
    Tetromino_t tetr = GetNextPiece();
    int color = SHAPES[tetr].color;

    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        if (SHAPES[tetr].matrix[i][j])
          prms->next[i][j] = color;
        else
          prms->next[i][j] = 0;
}

static void PauseGame(Params_t *prms) {
    static int prev_state = 0;
    if (*prms->pause) {
        *prms->pause = 0;
        *prms->state = prev_state;
    } else {
        *prms->pause = 1;
        prev_state = *prms->state;
        *prms->state = PAUSE;
    }
}

static void RestartGame(Params_t *prms){
  *prms->state = START;
}

static void ExitGame(Params_t *prms){
    free(prms->field);
    free(prms->next);
}