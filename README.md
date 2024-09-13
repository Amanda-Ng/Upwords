# Upwords Game Functions

## Overview
This repository contains a set of functions to support a variant of the game Upwords. While this is not the full game, the provided functions can be used as building blocks to implement the complete game.

## Functions
- ```GameState* initialize_game_state(const char *filename)```: Loads the initial game state from a file.
Assumes the file represents a valid Upwords board.
Initializes a GameState struct representing the game.
- ```GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed)```: Places tiles on the board either vertically or horizontally.
Direction: 'V' or 'v' for vertical, 'H' or 'h' for horizontal placement.
Tracks the number of tiles successfully placed.
- ```GameState* undo_place_tiles(GameState *game)```: Undoes the most recent successful tile placement.
If no successful tile placement has occurred, the function has no effect.
- ```void save_game_state(GameState *game, const char *filename)```: Saves the current game state to a file.
Includes the height of each tile stack, with proper formatting.
- ```void free_game_state(GameState *game)```: Frees all memory associated with the game, including any structures used for undo functionality.
