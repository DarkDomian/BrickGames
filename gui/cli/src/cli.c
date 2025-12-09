#include "cli.h"

int main(void) {
  // initialize ncurses
  initscr();    // Start curses mode
  raw();        // suspend, interrupt and quit char goes direct to the program
  noecho();     // block printing the user input
  keypad(stdscr, TRUE); // enable hande the F1..F12, arrows, etc.

  WINDOW *new_window;

  printw("Hello World !!!");	/* Print Hello World		  */
  refresh();			/* Print it on to the real screen */
  getch();			/* Wait for user input */
  endwin();			/* End curses mode		  */

  return 0;
}