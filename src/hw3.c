#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h" 

#define DEBUG(...) fprintf(stderr, "[          ] [ DEBUG ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, " -- %s()\n", __func__)

// // Function to initialize a stack
// GameStateStack *init_stack() {
//     GameStateStack *stack = (GameStateStack *)malloc(sizeof(GameStateStack));
//     stack->top = NULL;
//     return stack;
// }

// // Function to push a game state onto the stack
// void push(GameStateStack *stack, GameState *state) {
//     GameStateStackNode *node = (GameStateStackNode *)malloc(sizeof(GameStateStackNode));
//     node->state = state;
//     node->next = stack->top;
//     stack->top = node;
// }

// // Function to pop a game state from the stack
// GameState *pop(GameStateStack *stack) {
//     if (stack->top == NULL) {
//         return NULL;
//     }
//     GameStateStackNode *temp = stack->top;
//     stack->top = stack->top->next;
//     GameState *state = temp->state;
//     free(temp);
//     return state;
// }

// // Function to create a deep copy of a GameState
// GameState *copy_game_state(const GameState *original) {
//     GameState *copy = (GameState *)malloc(sizeof(GameState));

//     copy->rows = original->rows;
//     copy->cols = original->cols;

//     copy->board = (char **)malloc(copy->rows * sizeof(char *));
//     copy->stack_heights = (int **)malloc(copy->rows * sizeof(int *));

//     for (int i = 0; i < copy->rows; i++) {
//         copy->board[i] = (char *)malloc((copy->cols) * sizeof(char));
//         copy->stack_heights[i] = (int *)malloc(copy->cols * sizeof(int));

//         // Copy the contents of the board and stack_heights arrays
//         strcpy(copy->board[i], original->board[i]);
//         memcpy(copy->stack_heights[i], original->stack_heights[i], copy->cols * sizeof(int));
//     }

//     return copy;
// }


// void free_game_state_history(GameState *game) {
//     while (game->history->top != NULL) {
//         GameState *state = pop(game->history);
//         free_game_state(state);
//     }
//     free(game->history);
// }

void print_game(GameState* game){
    for(int i=0;i<game->rows; i++){
        for(int j=0; j<game->cols; j++){
            printf("%c", game->board[i][j]);
        }
        printf("\n");
    }
        for(int i=0;i<game->rows; i++){
        for(int j=0; j<game->cols; j++){
            printf("%d", game->stack_heights[i][j]);
        }
        printf("\n");
    }
}

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
    //  game->history = init_stack();

    // Initialize the GameState struct with the dimensions of the board
    game->rows = rows;
    game->cols = cols;
    game->board = (char **)malloc(rows * sizeof(char *));
    game->stack_heights = (int **)malloc(rows * sizeof(int *));

    // Read the contents of the file and initialize the game board
    // int row = 0;
    // int col = 0;
    // while ((ch = fgetc(file)) != EOF)
    // {
    //     if (ch == '\n')
    //     {
    //         row++;
    //         col = 0;
    //         continue;
    //     }
    //     // Allocate memory for the current row if not already allocated
    //     if (game->board[row] == NULL)
    //     {
    //         game->board[row] = (char *)malloc((cols) * sizeof(char));
    //         // game->board[row][cols] = '\0';
    //         game->stack_heights[row] = (int *)malloc(cols * sizeof(int));
    //     }
    //     // Assign the character to the corresponding cell on the board
    //     game->board[row][col] = ch;
    //     game->stack_heights[row][col] = (ch == '.') ? 0 : 1;
    //     col++;
    // }
    
    for(int i=0; i<game->rows;i++){
        game->board[i] = (char *)malloc((cols) * sizeof(char));
        game->stack_heights[i] = (int *)malloc(cols * sizeof(int));
        for(int j=0; j<game->cols;j++){
            ch = fgetc(file);
            if(ch=='\n'){
                ch = fgetc(file);
            }
            game->board[i][j] = ch;
            game->stack_heights[i][j] = (ch == '.') ? 0 : 1;
        }
    }
// print_game(game);
printf("Rows: %d", game->rows);
printf("Cols: %d", game->cols);
print_game(game);
    fclose(file);
    return game;
}

int is_legal_word(const char *word)
{
    // Open the dictionary file
    FILE *file = fopen("tests/words.txt", "r");

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
            game->stack_heights[i] = (int *)realloc(game->stack_heights[i], (game->cols + num_cols) * sizeof(int));
            // Initialize new columns with empty spaces
            for (int j = game->cols; j < game->cols + num_cols; j++) {
                game->board[i][j] = '.';
                game->stack_heights[i][j] = 0;
            }
        }
        // Update the number of columns
        game->cols += num_cols;
    }

    // Expand vertically
    if (num_rows > 0) {
        // Reallocate memory for the board to add new rows
        game->board = (char **)realloc(game->board, (game->rows + num_rows) * sizeof(char *));
        game->stack_heights = (int **)realloc(game->stack_heights, (game->rows + num_rows) * sizeof(int *));
        // Initialize new rows
        for (int i = game->rows; i < game->rows + num_rows; i++) {
            game->board[i] = (char *)malloc((game->cols) * sizeof(char));
            game->stack_heights[i] = (int *)malloc((game->cols) * sizeof(int));
            // Initialize new row with empty spaces
            for (int j = 0; j < game->cols; j++) {
                game->board[i][j] = '.';
                game->stack_heights[i][j] = 0;
            }
        }
        // Update the number of rows
        game->rows += num_rows;
    }
}

// Function to check if the move is valid
int is_valid_move(GameState *game, int row, int col, char direction, const char *tiles)
{

    //extract the word
    int start_index;
    int word_length = 0;
    if (direction == 'H'){
        start_index = col;
        for(int i=col-1; i>=0; i--){
            if(game->board[row][i]=='.'){
                break;
            }
            word_length++;
            start_index--;
        }
        word_length += strlen(tiles);
        for(int i=col+strlen(tiles); i<game->cols; i++){
            if(game->board[row][i]=='.'){
                break;
            }
            word_length++;
        }
    }else{
        start_index = row;
        for(int i=row-1; i>=0; i--){
            if(game->board[i][col]=='.'){
                break;
            }
            word_length++;
            start_index--;
        }
        word_length += strlen(tiles);
        for(int i=row+strlen(tiles); i<game->rows; i++){
            if(game->board[i][col]=='.'){
                break;
            }
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
        for(int i=row+strlen(tiles); i<game->rows; i++){
            if(game->board[i][col]=='.'){
                break;
            }
            word[i] = game->board[i][col];
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
        for(int i=col+strlen(tiles); i<game->cols; i++){
            if(game->board[row][i]=='.'){
                break;
            }
            word[i] = game->board[row][i];
        }
    }
    printf("word length: %d", word_length);
    printf("This is the word: %s", word);


    // if(!is_legal_word(word)){
    //     return 0;
    // }

    // if(num_tiles == (int)strlen(word)){
    //     //covers existing word
    //     return 0;
    // }

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

    // Check if row and col are valid
    if (row < 0 || col < 0 || row >= game->rows || col >= game->cols)
    {
        return game;
    }

    // Check if direction is valid
    if (direction != 'H' && direction != 'V')
    {
        return game;
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

    // Check if the move would exceed the maximum stack height
    if (direction == 'H')
    {
        // Check horizontally
        for (int j = col; j < game->cols; j++)
        {
            if (game->stack_heights[row][j] == 5)
            {
                return game; // Stack height would exceed maximum
            }
        }
        if ((col + num_tiles > game->cols))
        {
            // expand board
            int required_cols = col + strlen(tiles) - game->cols;
            expand_board(game, 0, required_cols);
        }
    }
    else
    {
        // Check vertically
        for (int i = row; i < game->rows; i++)
        {
            if (game->stack_heights[i][col] == 5)
            {
                return game; // Stack height would exceed maximum
            }
        }
        if ((row + num_tiles > game->rows))
        {
            // expand board
            int required_rows = row + num_tiles - game->rows;
            expand_board(game, required_rows, 0);
        }
    }

    // GameState *prev_state = copy_game_state(game);  // Create a copy of the current game state
    // push(game->history, prev_state);  // Push the current state onto the history stack

    *num_tiles_placed = 0;

    // Place tiles on the board
    if (direction == 'H')
    {
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
    {
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
print_game(game);
printf("Number tiles placed: %d\n", *num_tiles_placed);
    return game;
}

GameState* undo_place_tiles(GameState *game) {
    // (void)game;
    return game;
    // GameState *prev_state = pop(game->history);  // Pop the previous game state from the history

    // if (prev_state == NULL) {
    //     return game;  // No previous state, return the current state unchanged
    // }

    // game = prev_state;  // Set the game state to the previous state

    // return game;
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
