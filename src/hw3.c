#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)

GameState* initialize_game_state(const char *filename) {
    // (void)filename;
    // return NULL;
    FILE *file = fopen(filename, "r");

    // Determine the dimensions of the board by counting lines and characters per line
    int rows = 0;
    int cols = 0;
    int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF)
    {
        if (!isspace(ch))
        {
            cols++;
        }
    }
    rewind(file);
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            rows++;
        }
    }

    // Rewind the file pointer to read from the beginning
    rewind(file);

    // Allocate memory for the GameState struct
    GameState *game = (GameState *)malloc(sizeof(GameState));

    // Initialize the GameState struct with the dimensions of the board
    game->rows = rows;
    game->cols = cols;
    game->board = (char **)malloc(rows * sizeof(char *));
    game->stack_heights = (int **)malloc(rows * sizeof(int *));

    // Read the contents of the file and initialize the game board
    int row = 0;
    int col = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            row++;
            col = 0;
            continue;
        }
        // Allocate memory for the current row if not already allocated
        if (game->board[row] == NULL)
        {
            game->board[row] = (char *)malloc((cols) * sizeof(char));
            game->stack_heights[row] = (int *)malloc(cols * sizeof(int));
        }
        // Assign the character to the corresponding cell on the board
        game->board[row][col] = ch;
        game->stack_heights[row][col] = (ch == '.') ? 0 : 1;
        col++;
    }

    fclose(file);
    return game;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {
    (void)game;
    (void)row;
    (void)col;
    (void)direction;
    (void)tiles;
    (void)num_tiles_placed;
    return NULL;
}

GameState* undo_place_tiles(GameState *game) {
    (void)game;
    return NULL;
}

void free_game_state(GameState *game) {
    // (void)game;
    for (int i = 0; i < game->rows; i++)
    {
        free(game->board[i]);
        free(game->stack_heights[i]);
    }
    free(game->board);
    free(game->stack_heights);
    free(game);
}

void save_game_state(GameState *game, const char *filename) {
    // (void)game;
    // (void)filename;
        FILE *file = fopen(filename, "w");

    // Write the board state to the file
    for (int i = 0; i < game->rows; i++)
    {
        for (int j = 0; j < game->cols; j++)
        {
            // Write the top tile of the stack
            fputc(game->board[i][j], file);
        }
        fprintf(file, "\n"); // Newline at the end of each row
    }

    // Write the stack heights under the board state
    for (int i = 0; i < game->rows; i++)
    {
        for (int j = 0; j < game->cols; j++)
        {
            // Write the height of the stack
            fprintf(file, "%d", game->stack_heights[i][j]);
        }
        fprintf(file, "\n"); // Newline at the end of each row
    }

    fclose(file);
}
