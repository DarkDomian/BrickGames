#ifndef CLI_H
#define CLI_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "tetris_lib.h"

#define SUCCESS 0  ///< Return value indicating successful operation
#define ERROR 1    ///< Return value indicating operation failure

#define TICK_DELAY 50  ///< Delay in milliseconds between game ticks/updates

#define PLAY_FIELD_HEIGHT 20  ///< Height of the main game field in cells
#define PLAY_FIELD_WIDTH 10   ///< Width of the main game field in cells

// Key mappings for game controls (ASCII/KEY_* values)
#define KEY_ACTION 32  ///< Space bar - perform action
#define KEY_TERM 81    ///< 'Q' key - terminate game
#define KEY_PAUSE 80   ///< 'P' key - pause game
#define KEY_START 83   ///< 'S' key - start game

#define EMPTY_PIXEL "  "   ///< String representation of an empty cell
#define FILLED_PIXEL "[]"  ///< String representation of a filled cell

/**
 * @brief Container for ncurses windows used in the Brick Game UI
 *
 * This structure holds references to all windows needed for the game interface:
 * - Main game field
 * - Side information panel
 * - "Next piece" preview area
 */
typedef struct {
  WINDOW *game_field;  ///< Main window for the BrickGame playing field
  WINDOW *info_bar;    ///< Side window for score, level, and game info
  WINDOW *next_field;  ///< Subwindow within info_bar for next piece preview
} BrickUI;

/**
 * @brief Initialize ncurses mode and set up game windows
 *
 * Sets up ncurses environment and creates all windows needed for the Tetris UI.
 * Must be called before any other CLI functions.
 *
 * @param delay Input timeout in milliseconds (passed to wtimeout()).
 *              Negative = blocking read, 0 = non-blocking, Positive = timeout.
 * @param ui Pointer to BrickUI structure to populate with window references
 * @return SUCCESS (0) on success, ERROR (1) if ncurses initialization fails
 * @post ui->game_field, ui->info_bar, and ui->next_field are valid window
 * pointers
 * @warning Must be paired with BrickGameCleanup() to avoid memory leaks
 */
int BrickGameInit(const int delay, BrickUI *ui);

/**
 * @brief Clean up resources and terminate ncurses mode
 *
 * Deallocates all windows and cleans up ncurses state. Should be called
 * before program exit to restore terminal to normal state.
 *
 * @param ui Pointer to BrickUI structure containing windows to deallocate
 * @pre BrickGameInit() must have been called successfully
 * @post All windows are deallocated, terminal is restored to normal mode
 */
void BrickGameCleanup(BrickUI *ui);

/**
 * @brief Draw the static UI elements (boxes, labels, borders)
 *
 * Draws the initial static interface elements including:
 * - Border around game field
 * - Information labels (score, hi-score, level, next)
 * - Initial refresh of windows
 *
 * @param ui Pointer to BrickUI structure with initialized windows
 * @pre BrickGameInit() must have been called and windows must be valid
 */
void DrawUI(BrickUI *ui);

/**
 * @brief Update the dynamic information in the info bar
 *
 * Refreshes the changing game information (score, level, pause state)
 * in the side information panel.
 *
 * @param ui Pointer to BrickUI structure containing info_bar window
 * @param game_state Pointer to current game state information
 * @note This function does not refresh the entire UI, only info bar content
 */
void UpdateInfoBar(BrickUI *ui, GameInfo_t *game_state);

/**
 * @brief Render a 2D matrix to an ncurses window
 *
 * Converts a 2D integer matrix into a visual representation in the window.
 * Each cell is rendered as either FILLED_PIXEL or EMPTY_PIXEL based on its
 * value.
 *
 * @param screen Target ncurses window for output
 * @param matrix 2D integer array to render (0 = empty, non-zero = filled)
 * @param row Number of rows in the matrix
 * @param col Number of columns in the matrix (sic - intentional abbreviation)
 * @param shift_row Vertical offset for rendering within the window
 * @param shift_col Horizontal offset for rendering within the window (sic)
 * @note Assumes each matrix cell occupies 2 character positions horizontally
 * @warning matrix must be a valid pointer with at least row*col elements
 */
void PrintMatrix(WINDOW *screen, int **matrix, int row, int col, int shift_row,
                 int shift_col);

/**
 * @brief Main game event loop
 *
 * Handles input processing, game state updates, and UI refresh in a continuous
 * loop. The loop runs until a termination action is received.
 *
 * @param ui Pointer to BrickUI structure with initialized windows
 * @note This function blocks until game termination
 * @see CharToAction() for input handling
 * @see updateCurrentState() for game logic (external function)
 */
void GameLoop(BrickUI *ui);

/**
 * @brief Convert keyboard input to game action
 *
 * Maps ncurses key codes to internal game actions. Supports both
 * uppercase and lowercase key presses for letter-based commands.
 *
 * @param ch Key code received from ncurses input functions
 * @return Corresponding UserAction_t enum value
 * @note For letter keys (P, Q, S), both uppercase and lowercase are accepted
 * @see UserAction_t in tetris_lib.h for possible return values
 */
UserAction_t CharToAction(int ch);

#endif  // CLI_H