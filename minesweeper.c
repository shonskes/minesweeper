#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_ROW_AND_COL 0
#define BEGGINER_COLS 8
#define BEGGINER_ROWS 8
#define BEGGINER_MINES 10
#define INTERMIDIATE_COLS 16
#define INTERMIDIATE_ROWS 16
#define INTERMIDIATE_MINES 40
#define CRAZYYZY_COLS 30
#define CRAZYYZY_ROWS 16
#define CRAZYYZY_MINES 99

int rows, cols, mines = 0;
int game_menu();
int** creat_board();
int get_board_difficulty(int *valid_input, int difficulty);
void free_board(int **board);
void set_difficulty(int difficulty);
int ** init_board(int ** board);
int **create_revealed_board();
void print_debug_board(int **board);
void print_board_for_user(int **board, int **revealed);
void place_mines(int **board, int user_input_row, int user_input_col);
void calculate_adjecent_mines(int **borad);
void gameplay(int **board, int **revealed);
void reveal_adjacent_cells(int **board, int **revealed, int r, int c);

int main(void)
{
    srand(time(NULL));
    set_difficulty(game_menu());
    int **board = creat_board();
    int **revealed = create_revealed_board();
    init_board(board);
    gameplay(board, revealed);
    free_board(board);
    free_board(revealed);
    return 0;
}

int game_menu()
{
    int valid_input = 0;
    int difficulty = 0;
    printf("Hello!, Welcome to my minesweeper game.\nEnjoy ;)\n");
    while(!valid_input)
    {
        printf("Select a difficulty:\n");
        printf("1 - Beginner (8x8, 10 mines)\n");
        printf("2 - Intermediate (16x16, 40 mines)\n");
        printf("3 - Expert (30x16, 99 mines)\n");
        difficulty = get_board_difficulty(&valid_input, difficulty);
    }
    return difficulty;
}

int get_board_difficulty(int *valid_input, int difficulty)
{
    if(scanf("%d", &difficulty) == 1 && difficulty >= 1 && difficulty < 4)
    {
        while ((getchar()) != '\n');
        printf("\n");
        *valid_input = 1;
        return difficulty;
    }

    else
    {
        printf("Please dont break my game :(\nHere im giving you another chance :)\n");
        while (getchar() != '\n');
        return *valid_input;
    }
}

void set_difficulty(int difficulty)
{
    switch(difficulty)
    {
    case 1:
        rows = BEGGINER_ROWS;
        cols = BEGGINER_COLS;
        mines = BEGGINER_MINES;
        break;
    case 2:
        rows = INTERMIDIATE_ROWS;
        cols = INTERMIDIATE_COLS;
        mines = INTERMIDIATE_MINES;
        break;
    case 3:
        rows = CRAZYYZY_ROWS;
        cols = CRAZYYZY_COLS;
        mines = CRAZYYZY_MINES;
        break;
    default:
        break;
    }
}

int** creat_board()
{
    int **board;
    board = (int **)malloc(rows * sizeof(int *));
    int i = 0;
    for(i = 0; i < rows; i++)
    {
        board[i] = (int *)malloc(cols * sizeof(int));
    }
    return board;
}

int **create_revealed_board()
{
    int i = 0;
    int **revealed = (int **)malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++)
    {
        revealed[i] = (int *)calloc(cols, sizeof(int));
    }
    return revealed;
}

void free_board(int **board)
{
    int i = 0;
    for(i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);
}

int** init_board(int **board)
{
    int i,j = 0;
    for(i = 0; i < rows; i++) 
    {
        for(j = 0; j < cols; j++) 
        {
            board[i][j] = 0;
        }
    }
    return board;
}

void print_debug_board(int **board)
{
    int i, j;
    for(i = 0; i < rows; i++) 
    {
        for(j = 0; j < cols-1; j++) 
        {
            if(board[i][j] == -1)
            {
                printf("| X ");
            }

            else
            { 
                printf("| %d ", board[i][j]);
            }
        }

        if(board[i][cols-1] == -1)
        {
            printf("| X |\n");
        }
        
        else
        {
            printf("| %d |\n", board[i][cols-1]);
        }

        for(j = 0; j < ((cols * 4)+1); j++)
        {
            printf("-");
        }
        printf("\n");
    }
    printf("\n");
}

void place_mines(int **board, int user_input_row, int user_input_col)
{
    int placed = 0;
    int i = 0;
    int directions[9][2] =
    {
        {0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}, 
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    while(placed < mines)
    {
        int rand_num_for_row = rand() % (rows-MIN_ROW_AND_COL)+MIN_ROW_AND_COL;
        int rand_num_for_col = rand() % (cols-MIN_ROW_AND_COL)+MIN_ROW_AND_COL;

        int safe_zone = 0;
        for (i = 0; i < 9; i++)
        {
            if (rand_num_for_row == user_input_row + directions[i][0] && rand_num_for_col == user_input_col + directions[i][1]) {
                safe_zone = 1;
                break;
            }
        }
        if(!safe_zone && board[rand_num_for_row][rand_num_for_col] != -1) {
            board[rand_num_for_row][rand_num_for_col] = -1;
            placed++;
        }
    }
}

void calculate_adjecent_mines(int **board)
{
    int i,j,d = 0;
    int directions[8][2] = 
    {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            if(board[i][j] == -1)
            {
                continue;
            }

            int mine_count = 0;
            for(d = 0; d < 8; d++)
            {
                int new_row = i + directions[d][0];
                int new_col = j + directions[d][1];

                if(new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols)
                {
                    if(board[new_row][new_col] == -1)
                    {
                        mine_count++;
                    }
                }
            }
            board[i][j] = mine_count;
        }
    }
}

void print_board_for_user(int **board, int **revealed)
{
    int i, j;
    for (i = 0; i < rows; i++) 
    {
        for (j = 0; j < cols; j++) 
        {
            printf("| ");
            
            if(revealed[i][j] == -1)
            {
                printf("F ");
            } 

            else if (revealed[i][j] == 1)
            {
                if (board[i][j] == -1)
                    printf("X ");
                else
                    printf("%d ", board[i][j]);
            }
            
            else
            {
                printf("O ");
            }
        }

        printf("|\n");

        for (j = 0; j < ((cols * 4) + 1); j++)
        {
            printf("-");
        }
        printf("\n");
    }
}

void gameplay(int **board, int **revealed)
{
    int user_input_row, user_input_col = 0;
    int unrevealed_cells = rows * cols - mines;
    char action;
    int first_turn = 1;
    int i, j = 0;
    int flagged_mines = 0;

    while (1)
    {
        print_board_for_user(board, revealed);
        printf("Enter action (R for reveal, F for flag/unflag), row, and column (format: R 2 3) the board starts at 0, 0 meaning 7, 7 is the limit\n> ");
        while (scanf(" %c %d %d", &action, &user_input_row, &user_input_col) != 3 || 
               user_input_row < 0 || user_input_row >= rows || 
               user_input_col < 0 || user_input_col >= cols || 
               (action != 'R' && action != 'F')) 
        {
            printf("Invalid input! Use R/F followed by row and column.\n> ");
            while (getchar() != '\n');
        }

        if (action == 'F')
        {
            if (revealed[user_input_row][user_input_col] == 0)
            {
                revealed[user_input_row][user_input_col] = -1;
                if (board[user_input_row][user_input_col] == -1)
                {
                    flagged_mines++;
                }
            }

            else if (revealed[user_input_row][user_input_col] == -1)
            {
                revealed[user_input_row][user_input_col] = 0;
                if (board[user_input_row][user_input_col] == -1)
                {
                    flagged_mines--;
                }
            }

            else
            {
                printf("You can't flag an already revealed cell!\n");
            }
            continue;
        }

        if(first_turn)
        {
            place_mines(board, user_input_row, user_input_col);
            calculate_adjecent_mines(board);
            first_turn = 0;
        }
        
        if (revealed[user_input_row][user_input_col] == -1) {
            printf("You can't reveal a flagged cell! Unflag it first.\n");
            continue;
        }

        if (board[user_input_row][user_input_col] == -1) {
            print_debug_board(board);
            printf("HEHEHEHE You hit a mine Try not to do that next time ;) \n");
            break;
        }

        reveal_adjacent_cells(board, revealed, user_input_row, user_input_col);

        if (flagged_mines == mines)
        {
            print_board_for_user(board, revealed);
            printf("Congratulations! You won by flagging all mines! Hope you enjoyed\n");
            break;
        }
    }
}

void reveal_adjacent_cells(int **board, int **revealed, int r, int c)
{
    int d = 0;
    if(r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c])
    {
        return;
    }

    revealed[r][c] = 1;

    if (board[r][c] == 0)
    {
        int directions[8][2] =
        {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1},
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        };

        for (d = 0; d < 8; d++)
        {
            int new_r = r + directions[d][0];
            int new_c = c + directions[d][1];
            reveal_adjacent_cells(board, revealed, new_r, new_c);
        }
    }
}

