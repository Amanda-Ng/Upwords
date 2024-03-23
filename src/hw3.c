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

int is_legal_word(const char *word)
{
    // Open the dictionary file
    FILE *file = fopen("words.txt", "r");

    // Convert the provided word to lowercase for case-insensitive comparison
    char *lowercase_word = (char *)malloc(strlen(word) + 1);
    for (int i = 0; word[i] != '\0'; i++)
    {
        lowercase_word[i] = tolower(word[i]);
    }
    lowercase_word[strlen(word)] = '\0';

    // Read each word from the dictionary file and compare it with the provided word
    char file_word[100];
    while (fscanf(file, "%s", file_word) != EOF)
    {
        // Convert the file word to lowercase for comparison
        char *lowercase_file_word = (char *)malloc(strlen(file_word) + 1);
        for (int i = 0; file_word[i] != '\0'; i++)
        {
            lowercase_file_word[i] = tolower(file_word[i]);
        }
        lowercase_file_word[strlen(file_word)] = '\0';
        // Check if the word is in the file
        if (strcmp(lowercase_file_word, lowercase_word) == 0)
        {
            free(lowercase_word);
            free(lowercase_file_word);
            fclose(file);
            return 1;
        }
        free(lowercase_file_word);
    }
    free(lowercase_word);
    fclose(file);
    return 0;
}

// Function to expand the game board
void expand_board(GameState *game, int num_rows, int num_cols) {
    // Expand horizontally
    if (num_cols > 0) {
        // Reallocate memory for each row in the board
        for (int i = 0; i < game->rows; i++) {
            game->board[i] = (char *)realloc(game->board[i], (game->cols + num_cols) * sizeof(char));
            // Initialize new columns with empty spaces
            for (int j = game->cols; j < game->cols + num_cols; j++) {
                game->board[i][j] = '.';
            }
        }
        // Update the number of columns
        game->cols += num_cols;
    }

    // Expand vertically
    if (num_rows > 0) {
        // Reallocate memory for the board to add new rows
        game->board = (char **)realloc(game->board, (game->rows + num_rows) * sizeof(char *));
        // Initialize new rows
        for (int i = game->rows; i < game->rows + num_rows; i++) {
            game->board[i] = (char *)malloc((game->cols) * sizeof(char));
            // Initialize new row with empty spaces
            for (int j = 0; j < game->cols; j++) {
                game->board[i][j] = '.';
            }
        }
        // Update the number of rows
        game->rows += num_rows;
    }
}

// Function to check if the move is valid
int is_valid_move(GameState *game, int row, int col, char direction, const char *tiles)
{
    // Check if row and col are valid
    if (row < 0 || col < 0 || row >= game->rows || col >= game->cols)
    {
        return 0;
    }

    // Check if direction is valid
    if (direction != 'H' && direction != 'V')
    {
        return 0;
    }

    //extract the word
    int start_index;
    int word_length = 0;
    if (direction == 'H'){
        start_index = col;
        for(int i=col-1; game->board[row][i]!='.'; i--){
            word_length++;
            start_index--;
        }
        word_length += strlen(tiles);
        for(int i=col+strlen(tiles); game->board[row][i]!='.'; i++){
            word_length++;
        }
    }else{
        start_index = row;
        for(int i=row-1; game->board[i][col]!='.'; i--){
            word_length++;
            start_index--;
        }
        word_length += strlen(tiles);
        for(int i=row+strlen(tiles); game->board[i][col]!='.'; i++){
            word_length++;
        }
    }
    char *word = malloc(word_length + 1);
    if (direction == 'H'){
        for(int i=0; i<word_length; i++){
            if(start_index + i < game->cols){
                word[i] = game->board[row][start_index + i];
            }
        }
        for(int i=0; i<(int)strlen(tiles); i++){
            if(*(tiles+i) != ' '){
                word[col - start_index + i] = *(tiles+i);
            }
        }
    }else{
        for(int i=0; i<word_length; i++){
            if(start_index + i < game->rows){
                word[i] = game->board[start_index + i][col];
            }
        }
        for(int i=0; i<(int)strlen(tiles); i++){
            if(*(tiles+i) != ' '){
                word[row - start_index + i] = *(tiles+i);
            }
        }
    }


    if(!is_legal_word(word)){
        return 0;
    }

    // Determine the number of tiles to be placed
    int num_tiles = 0;
    for (const char *tile = tiles; *tile != '\0'; tile++)
    {
        if (*tile != ' ')
        {
            num_tiles++;
        }
    }

    if(num_tiles == (int)strlen(word)){
        //covers existing word
        return 0;
    }

    // Check if the move would exceed the maximum stack height
    if (direction == 'H')
    {
        // Check horizontally
        if ((col + num_tiles > game->cols) && is_legal_word(word))
        {
            // expand board
            int required_cols = col + num_tiles - game->cols - 1;
            expand_board(game, 0, required_cols);
        }
        for (int j = col; j < col + num_tiles; j++)
        {
            if (game->stack_heights[row][j] == 5)
            {
                return 0; // Stack height would exceed maximum
            }
        }
    }
    else
    {
        // Check vertically
        if ((row + num_tiles > game->rows) && is_legal_word(word))
        {
            // expand board
            int required_rows = row + num_tiles - game->rows - 1;
            expand_board(game, required_rows, 0);
        }
        for (int i = row; i < row + num_tiles; i++)
        {
            if (game->stack_heights[i][col] == 5)
            {
                return 0; // Stack height would exceed maximum
            }
        }
    }

    return 1;
}

GameState* place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed) {
    // (void)game;
    // (void)row;
    // (void)col;
    // (void)direction;
    // (void)tiles;
    // (void)num_tiles_placed;
    // return NULL;

    if (!is_valid_move(game, row, col, direction, tiles))
    {
        return game;
    }

    *num_tiles_placed = 0;

    // Determine the number of tiles to be placed
    int num_tiles = 0;
    for (const char *tile = tiles; *tile != '\0'; tile++)
    {
        if (*tile != ' ')
        {
            num_tiles++;
        }
    }

    // Place tiles on the board
    if (direction == 'H')
    {
        // for (int j = col; j < col + num_tiles; j++)
        // {
        //     // Determine the height of the stack at the current position
        //     int stack_height = game->stack_heights[row][j];
        //     // Update the stack height
        //     game->stack_heights[row][j]++;
        //     // Place the tile on the board
        //     game->board[row][j] = tiles[*num_tiles_placed++];
        //     // Increment the number of tiles placed
        //     (*num_tiles_placed)++;
        // }
        for (int j = 0; j < (int)strlen(tiles); j++)
        {
            // Determine the height of the stack at the current position
            // int stack_height = game->stack_heights[row][j];
            if(*(tiles+j) != ' '){
                // Update the stack height
                game->stack_heights[row][col+j]++;
                // Place the tile on the board
                game->board[row][col+j] = *(tiles+j);
                // Increment the number of tiles placed
                (*num_tiles_placed)++;
            }
        }
    }
    else
    { // direction == 'V'
        // for (int i = row; i < row + num_tiles; i++)
        // {
        //     // Determine the height of the stack at the current position
        //     int stack_height = game->stack_heights[i][col];
        //     // Update the stack height
        //     game->stack_heights[i][col]++;
        //     // Place the tile on the board
        //     game->board[i][col] = tiles[*num_tiles_placed++];
        //     // Increment the number of tiles placed
        //     (*num_tiles_placed)++;
        //     // Update stack heights for tiles underneath if stacked
        // }
        for (int j = 0; j < (int)strlen(tiles); j++)
        {
            // Determine the height of the stack at the current position
            // int stack_height = game->stack_heights[row][j];
            if(*(tiles+j) != ' '){
                // Update the stack height
                game->stack_heights[row+j][col]++;
                // Place the tile on the board
                game->board[row+j][col] = *(tiles+j);
                // Increment the number of tiles placed
                (*num_tiles_placed)++;
            }
        }
    }

    return game;
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
