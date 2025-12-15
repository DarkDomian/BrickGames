#include "cli.h"

int main(void) {
  BrickUI ui = {
    .game_field = NULL,
    .info_bar = NULL
  };

  if (BrickGameInit(50, &ui) == ERROR) {
    fprintf(stderr, "Faild to initialize ncurses mode...");
    return ERROR;
  }
  
  DrawUI(&ui);
  
  // GameLoop
  char ch;
  while ((ch = wgetch(ui.game_field)) != 'q') {
    continue;
  }

  BrickGameCleanup(&ui);
}


int BrickGameInit(const int delay, BrickUI *ui) {
  if (initscr() == NULL) return ERROR;
  
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(delay);

  ui->game_field = newwin(PLAY_FIELD_HEIGHT + 2, PLAY_FIELD_WIDTH * 2 + 2, 0, 0);
  ui->info_bar = newwin(PLAY_FIELD_HEIGHT + 2, 12, 0, PLAY_FIELD_WIDTH * 2 + 2);
  
  return SUCCESS;
}

void BrickGameCleanup(BrickUI *ui) {
  delwin(ui->game_field);
  delwin(ui->info_bar);
  endwin();
}

void DrawUI(BrickUI *ui) {
  box(ui->game_field, 0, 0);
  box(ui->info_bar, 0, 0);

  mvwprintw(ui->info_bar, 1, 2, "%8s", "score");
  // score = derwin(side_bar, 1, 8, 2, 2);
  mvwprintw(ui->info_bar, 4, 2, "%8s", "hi-score");
  // hi_score = derwin(side_bar, 1, 8, 5, 2);
  mvwprintw(ui->info_bar, 7, 2, "%8s", "next");
  // next = derwin(side_bar, 4, 8, 8, 2);
  
  mvwprintw(ui->info_bar, 19, 2, "%8s", "pause");

  wrefresh(ui->game_field);
  wrefresh(ui->info_bar);
}