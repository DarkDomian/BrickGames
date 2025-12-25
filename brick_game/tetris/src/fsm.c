#include "fsm.h"

typedef void (*FunctionCallback_t)(CoreGameState_t *game);

static void Spawn(CoreGameState_t *game);
static void PauseGame(CoreGameState_t *game);
static void RestartGame(CoreGameState_t *game);
static void ExitGame(CoreGameState_t *game);

const FunctionCallback_t kFSMTable[3][9] = {
    {Spawn, PauseGame, ExitGame, NULL, NULL, NULL, NULL, NULL, NULL},
    {PauseGame, NULL, ExitGame, PauseGame, PauseGame, PauseGame, PauseGame,
     PauseGame, NULL},
    {RestartGame, PauseGame, ExitGame, RestartGame, RestartGame, RestartGame,
     RestartGame, RestartGame, RestartGame}};

void SigAct(CoreGameState_t *game) {
  FunctionCallback_t act = NULL;

  if (!game->usr_input->fresh) game->usr_input->action = NOSIG;

  act = kFSMTable[game->state][game->usr_input->action];

  if (act) act(game);
}

static void Spawn(CoreGameState_t *game) {
  Tetromino_t tetr = GetNextPiece();
  int color = kTetrShape[tetr].color;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (kTetrShape[tetr].matrix[i][j])
        game->next[i][j] = color;
      else
        game->next[i][j] = 0;
}

static void PauseGame(CoreGameState_t *game) {
  static int prev_state = 0;
  if (game->info.pause) {
    game->info.pause = 0;
    game->state = prev_state;
  } else {
    game->info.pause = 1;
    prev_state = game->state;
    game->state = PAUSE;
  }
}

static void RestartGame(CoreGameState_t *game) { game->state = START; }

static void ExitGame(CoreGameState_t *game) {
  free(game->field);
  free(game->next);
}