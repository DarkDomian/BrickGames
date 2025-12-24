/**
 * @file tetris_lib.h
 * @brief Main API header for the Tetris BrickGame game library
 *
 * This header defines the core API for tetris game in the BrickGame collection.
 * It provides structures and functions that allow the interface to interact
 * with game logic, handle user input, and retrieve game state for rendering.
 * The API follows BrickGame specification, it's designed to be
 * forward-compatible for future game implementations across different interface
 * types (console, desktop, etc.).
 */

#ifndef TETRIS_LIB_H
#define TETRIS_LIB_H

#include <stdbool.h>

/**
 * @brief Enumeration of possible user actions from physical buttons
 *
 * Represents the eight physical buttons available in the console interface.
 * Each action corresponds to a specific button press that games can respond to.
 */
typedef enum {
  Start,     /**< Start button - begins or resumes the game */
  Pause,     /**< Pause button - toggles game pause state */
  Terminate, /**< Terminate button - ends the current game session */
  Left,      /**< Left arrow button - moves game element left */
  Right,     /**< Right arrow button - moves game element right */
  Up,        /**< Up arrow button - rotates or moves game element up */
  Down,      /**< Down arrow button - accelerates downward movement */
  Action     /**< Action button - game-specific primary action */
} UserAction_t;

/**
 * @brief Structure containing all game state information for rendering
 *
 * This structure is returned by updateCurrentState() and contains all
 * necessary data for the interface to render the current game state.
 * The playfield is a 20x10 matrix (rows x columns), with additional
 * information displayed in the sidebar.
 */
typedef struct {
  /**
   * @brief Main playfield matrix (20 rows x 10 columns)
   *
   * Each element represents a "pixel" in the playfield:
   * - 0: Empty cell
   * - Non-zero: Filled cell (specific values may indicate colors/types)
   *
   * @note Memory for this matrix is managed internally by the game library
   */
  int **field;

  /**
   * @brief Preview of next game element
   *
   * Matrix showing the next piece or element that will appear.
   * Size and format are game-dependent. For games without preview,
   * this may be NULL or a placeholder matrix.
   */
  int **next;

  int score;      /**< Current player score */
  int high_score; /**< Highest score achieved in this session */
  int level;      /**< Current game level (affects difficulty) */
  int speed;      /**< Current game speed (ms per update) */
  int pause;      /**< Pause state: 0 = playing, 1 = paused */
} GameInfo_t;

/**
 * @brief Processes user input from physical buttons
 *
 * This function is called by the interface when a button is pressed or
 * released. It translates physical button actions into game logic responses.
 *
 * @param action The type of button action (from UserAction_t enum)
 * @param hold Indicates whether the button is being held down (true) or just
 * pressed (false)
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Updates and retrieves the current game state
 *
 * This function should be called at regular intervals (determined by the
 * current game speed) to advance the game logic and get updated state
 * for rendering. It handles automatic game progression (like piece falling
 * in Tetris when timer expires).
 *
 * @return GameInfo_t structure containing the complete current game state
 *
 * @note The returned structure contains pointers to internal data. The
 *       interface should not modify or free these pointers. The data is
 *       valid until the next call to updateCurrentState().
 * @warning This function should not be called from multiple threads
 *          simultaneously without proper synchronization.
 */
GameInfo_t updateCurrentState();

#endif /* TETRIS_LIB_H */