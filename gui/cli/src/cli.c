#include "cli.h"

int main(void) {
  BrickUI ui = {.game_field = NULL, .info_bar = NULL, .next_field = NULL};

  if (BrickGameInit(TICK_DELAY, &ui) == ERROR) {
    fprintf(stderr, "Faild to initialize ncurses mode...");
    return ERROR;
  }

  DrawUI(&ui);

  GameLoop(&ui);

  BrickGameCleanup(&ui);
}

int BrickGameInit(const int delay, BrickUI *ui) {
  if (initscr() == NULL) return ERROR;

  noecho();
  curs_set(0);

  raw();

  ui->game_field =
      newwin(PLAY_FIELD_HEIGHT + 2, PLAY_FIELD_WIDTH * 2 + 2, 0, 0);
  ui->info_bar = newwin(PLAY_FIELD_HEIGHT + 2, 12, 0, PLAY_FIELD_WIDTH * 2 + 2);
  ui->next_field = derwin(ui->info_bar, 4, 8, 11, 2);

  keypad(ui->game_field, TRUE);
  wtimeout(ui->game_field, delay);

  return SUCCESS;
}

void BrickGameCleanup(BrickUI *ui) {
  delwin(ui->game_field);
  delwin(ui->next_field);
  delwin(ui->info_bar);
  endwin();
}

void DrawUI(BrickUI *ui) {
  box(ui->game_field, 0, 0);

  // TODO: maybe be draw the titles while they present into GameInfo_t
  mvwprintw(ui->info_bar, 1, 2, "%8s", "score");
  mvwprintw(ui->info_bar, 4, 2, "%8s", "hi-score");
  mvwprintw(ui->info_bar, 7, 2, "%8s", "level");
  mvwprintw(ui->info_bar, 10, 2, "%8s", "next");

  wrefresh(ui->game_field);
  wrefresh(ui->info_bar);
}

void PrintMatrix(WINDOW *screen, int **matrix, int row, int col, int shift_row,
                 int shift_col) {
  if (matrix == NULL) return;
  // wclear(screen);

  for (int y = 0; y < row; ++y) {
    for (int x = 0; x < col; ++x) {
      if (matrix[y][x])
        mvwaddnstr(screen, y + shift_row, x * 2 + shift_col, FILLED_PIXEL, 2);
      else
        mvwaddnstr(screen, y + shift_row, x * 2 + shift_col, EMPTY_PIXEL, 2);
    }
  }

  wrefresh(screen);

  return;
}

UserAction_t CharToAction(int ch) {
  UserAction_t act = Start;

  if (ch == KEY_LEFT)
    act = Left;
  else if (ch == KEY_RIGHT)
    act = Right;
  else if (ch == KEY_UP)
    act = Up;
  else if (ch == KEY_DOWN)
    act = Down;
  else if (ch == KEY_ACTION)
    act = Action;
  else if (ch == KEY_PAUSE || ch == (KEY_PAUSE + 32))
    act = Pause;
  else if (ch == KEY_START || ch == (KEY_START + 32))
    act = Start;
  else if (ch == KEY_TERM || ch == (KEY_TERM + 32))
    act = Terminate;

  return act;
}

void GameLoop(BrickUI *ui) {
  int prev_ch = ERR, curr_ch = ERR;
  UserAction_t act = Start;

  GameInfo_t game_state;
  while (1) {
    if ((curr_ch = wgetch(ui->game_field)) != ERR) {
      act = CharToAction(curr_ch);
      userInput(act, (prev_ch == curr_ch) ? true : false);
    }

    if (act == Terminate) break;
    prev_ch = curr_ch;

    // some function for update UI
    game_state = updateCurrentState();
    PrintMatrix(ui->game_field, game_state.field, 20, 10, 1, 1);
    UpdateInfoBar(ui, &game_state);
    PrintMatrix(ui->next_field, game_state.next, 4, 4, 0, 0);
  }

  return;
}

void UpdateInfoBar(BrickUI *ui, GameInfo_t *game_state) {
  // under `score` title
  mvwprintw(ui->info_bar, 2, 2, "%8d", game_state->score);
  // under `hi-score` title
  mvwprintw(ui->info_bar, 5, 2, "%8d", game_state->high_score);
  // level
  mvwprintw(ui->info_bar, 8, 2, "%8d", game_state->level);
  // pause state
  mvwprintw(ui->info_bar, 20, 2, "%8s", game_state->pause ? "pause" : "");

  wrefresh(ui->info_bar);
}