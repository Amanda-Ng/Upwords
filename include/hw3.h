#include <stdlib.h>

// typedef struct GameState GameState;

// typedef struct GameStateStackNode {
//     GameState *state;
//     struct GameStateStackNode *next;
// } GameStateStackNode;

// typedef struct {
//     GameStateStackNode *top;
// } GameStateStack;

typedef struct GameState
{
    // define your struct here
    char **board;
    int **stack_heights;
    int rows;
    int cols;
    // GameStateStack *history;
} GameState;

GameState* initialize_game_state(const char *filename);
GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed);
GameState* undo_place_tiles(GameState *game);
void free_game_state(GameState *game);
void save_game_state(GameState *game, const char *filename);
