#ifndef CLI_H
#define CLI_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

// #include "tetris_lib.h"

#define SUCCESS 0
#define ERROR 1

#define PLAY_FIELD_HEIGHT 20
#define PLAY_FIELD_WIDTH 10

/** Structure with pointers to gaming windows, exists for easy data share between functions */
typedef struct {
    WINDOW *game_field;
    WINDOW *info_bar;
} BrickUI;

/**
 * @brief initialize curses mode and setup the `BrickUI` by allocate memory for windows
 *
 * @param delay passed to `timeout()` ncurses function
 * @param ui pointer to BrickUI for windows managment
 * @note If delay is negative, curses uses a blocking read, waiting indefinitely
 * for input. If delay is zero, the read is non-blocking; an input character
 * reading function returns ERR if no input is pending. If delay is positive, an
 * input character reading function blocks for delay milliseconds, and returns
 * ERR if the delay elapses and there is still no input pending.
 */
int BrickGameInit(const int delay, BrickUI *ui);

/**
 * @brief Call at the end of work. Deallocate windwos memory, fihish curses mode.
 * 
 * @param ui pointer to BrickUI for windows managment 
 */
void BrickGameCleanup(BrickUI *ui);

/**
 * @brief Called once for drawing game-HUD, boxes around game field and titels for information
 * 
 * @param ui pointer to BrickUI for windows managment
 */
void DrawUI(BrickUI *ui);

#endif  // CLI_H