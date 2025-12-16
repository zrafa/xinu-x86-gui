#include <xinu.h>
#include <stdio.h>

// ANSI escape codes for cursor control and colors
#define CLEAR_SCREEN "\033[2J"
#define CURSOR_HOME "\033[H"
#define CURSOR_POS(row, col) printf("\033[%d;%dH", (row), (col))
#define CLEAR_LINE "\033[K"
#define COLOR_RED "\033[31m" // bright red for X
// #define COLOR_BLUE    "\033[34m"   // it look bad on black screen
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"

char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
char current_player = 'X';
int last_move = -1;

typedef struct
{
    int x_wins;
    int o_wins;
    int ties;
} TicTacToeScore;

TicTacToeScore tictactoe_score = {0, 0, 0};

const char *get_cell_color(int pos)
{
    if (board[pos] == 'X')
        return COLOR_RED;
    if (board[pos] == 'O')
        return COLOR_GREEN;
    return COLOR_YELLOW; // Available positions
}

void print_board()
{
    printf("\n");
    printf("  %s+---+---+---+%s\n", COLOR_CYAN, COLOR_RESET);

    for (int row = 0; row < 3; row++)
    {
        printf("  %s|%s", COLOR_CYAN, COLOR_RESET);
        for (int col = 0; col < 3; col++)
        {
            int pos = row * 3 + col;
            const char *color = get_cell_color(pos);
            const char *bold = (pos == last_move) ? COLOR_BOLD : "";
            printf(" %s%s%c%s %s|%s", bold, color, board[pos], COLOR_RESET, COLOR_CYAN, COLOR_RESET);
        }
        printf("\n");

        if (row < 2)
        {
            printf("  %s+---+---+---+%s\n", COLOR_CYAN, COLOR_RESET);
        }
    }

    printf("  %s+---+---+---+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("\n");
}

int check_winner()
{
    int win_conditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // rows
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8}, // columns
        {0, 4, 8},
        {2, 4, 6} // diagonals
    };

    for (int i = 0; i < 8; i++)
    {
        if (board[win_conditions[i][0]] == board[win_conditions[i][1]] &&
            board[win_conditions[i][1]] == board[win_conditions[i][2]])
        {
            return 1;
        }
    }
    return 0;
}

int is_board_full()
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] != 'X' && board[i] != 'O')
        {
            return 0;
        }
    }
    return 1;
}

int is_valid_move(int position)
{
    if (position < 0 || position > 8)
    {
        return 0;
    }
    return (board[position] != 'X' && board[position] != 'O');
}

void make_move(int position)
{
    board[position] = current_player;
    last_move = position;
}

void print_score()
{
    printf("  %s+======== SCORE ===========+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s|%s  %sX: %2d%s | %sO: %2d%s | Tie: %2d %s|%s\n",
           COLOR_CYAN, COLOR_RESET,
           COLOR_RED, tictactoe_score.x_wins, COLOR_RESET,
           COLOR_GREEN, tictactoe_score.o_wins, COLOR_RESET,
           tictactoe_score.ties,
           COLOR_CYAN, COLOR_RESET);
    printf("  %s+==========================+%s\n", COLOR_CYAN, COLOR_RESET);
}

void print_status(int mode)
{
    printf("  %s===============================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  Current Player: %s%s%c%s  |  Mode: %s\n",
           COLOR_BOLD,
           current_player == 'X' ? COLOR_RED : COLOR_GREEN,
           current_player,
           COLOR_RESET,
           mode == 1 ? "Human vs Human" : "Human vs Computer");
    printf("  %s===============================================%s\n\n", COLOR_CYAN, COLOR_RESET);
}

// Optimized update functions that only redraw changed parts
// Update only a single cell - most efficient!
void update_cell_only(int position)
{
    int row = position / 3;
    int col = position % 3;

    // Calculate screen row (8 for board start + row*2 for grid spacing)
    int screen_row = 8 + 3 + (row * 2) - 1;
    // Calculate screen column (4 for border + col*4 for cell width)
    int screen_col = 4 + (col * 4) + 1;

    CURSOR_POS(screen_row, screen_col);
    const char *color = get_cell_color(position);
    printf("%s%c%s\n", color, board[position], COLOR_RESET);
    CURSOR_POS(24, 80); // Move cursor off-screen to hide it
}

// Update only the current player text - super fast!
void update_player_only(int mode)
{
    CURSOR_POS(18, 19); // Position at "Current Player: " text
    const char *color = current_player == 'X' ? COLOR_RED : COLOR_GREEN;
    printf("%s%s%c%s\n", COLOR_BOLD, color, current_player, COLOR_RESET);
}

void update_score_only()
{
    CURSOR_POS(5, 1);
    printf(CLEAR_LINE);
    printf("  %s+======== SCORE ===========+\n%s", COLOR_CYAN, COLOR_RESET);
    CURSOR_POS(6, 1);
    printf(CLEAR_LINE);
    printf("  %s|%s  %sX: %2d%s | %sO: %2d%s | Tie: %2d %s|%s\n",
           COLOR_CYAN, COLOR_RESET,
           COLOR_RED, tictactoe_score.x_wins, COLOR_RESET,
           COLOR_GREEN, tictactoe_score.o_wins, COLOR_RESET,
           tictactoe_score.ties,
           COLOR_CYAN, COLOR_RESET);
    CURSOR_POS(7, 1);
    printf(CLEAR_LINE);
    printf("  %s+==========================+%s\n", COLOR_CYAN, COLOR_RESET);
}

void clear_message_area()
{
    CURSOR_POS(20, 1);
    printf(CLEAR_LINE);
    CURSOR_POS(21, 1);
    printf(CLEAR_LINE);
    CURSOR_POS(22, 1);
    printf(CLEAR_LINE);
}

void show_message(const char *msg, const char *color)
{
    clear_message_area();
    CURSOR_POS(21, 1);
    printf("  %s%s%s", color, msg, COLOR_RESET);
}

typedef struct
{
    int position;
    int score;
} Move;

int get_available_count()
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (is_valid_move(i))
        {
            count++;
        }
    }
    return count;
}

Move minimax(char player, char max_player)
{
    Move best_move;
    char other_player = (player == 'X') ? 'O' : 'X';

    // Check if previous move was a winner
    if (current_player == 'X')
    {
        current_player = 'O';
    }
    else
    {
        current_player = 'X';
    }

    if (check_winner())
    {
        current_player = player;
        char winner = other_player;
        int score = (winner == max_player) ? (get_available_count() + 1) : -(get_available_count() + 1);
        best_move.position = -1;
        best_move.score = score;
        return best_move;
    }

    current_player = player;

    // Check for tie
    if (is_board_full())
    {
        best_move.position = -1;
        best_move.score = 0;
        return best_move;
    }

    // Initialize best move
    if (player == max_player)
    {
        best_move.score = -99999; // Maximize
    }
    else
    {
        best_move.score = 99999; // Minimize
    }
    best_move.position = -1;

    // Try all possible moves
    for (int i = 0; i < 9; i++)
    {
        if (is_valid_move(i))
        {
            char original = board[i];
            char original_winner = current_player;

            // Make move
            board[i] = player;
            if (check_winner())
            {
                current_player = player;
            }

            // Simulate game with minimax
            Move sim_move = minimax(other_player, max_player);
            sim_move.position = i;

            // Undo move
            board[i] = original;
            current_player = original_winner;

            // Update best move
            if (player == max_player)
            {
                if (sim_move.score > best_move.score)
                {
                    best_move = sim_move;
                }
            }
            else
            {
                if (sim_move.score < best_move.score)
                {
                    best_move = sim_move;
                }
            }
        }
    }

    return best_move;
}

int computer_move()
{
    // Use minimax algorithm for smart AI
    if (get_available_count() == 9)
    {
        // First move: choose randomly
        int available[9];
        int count = 0;
        for (int i = 0; i < 9; i++)
        {
            if (is_valid_move(i))
            {
                available[count++] = i;
            }
        }
        return available[rand() % count];
    }

    // Use minimax for subsequent moves
    Move best_move = minimax('O', 'O');
    return best_move.position;
}

void clear_and_home()
{
    printf(CLEAR_SCREEN);
    printf(CURSOR_HOME);
}

void reset_board()
{
    for (int i = 0; i < 9; i++)
    {
        board[i] = '1' + i;
    }
    current_player = 'X';
    last_move = -1;
}

void print_title()
{
    printf("\n");
    printf("  %s+===============================+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s|%s      %s%sTIC TAC TOE GAME%s         %s|%s\n",
           COLOR_CYAN, COLOR_RESET, COLOR_BOLD, COLOR_GREEN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("  %s+===============================+%s\n", COLOR_CYAN, COLOR_RESET);
}

void play_game(int mode)
{
    reset_board();
    int move;
    int first_draw = 1;

    // randomly decide who goes first
    clear_and_home();
    print_title();
    srand(clktime);
    if(mode == 1){
        current_player = (rand() % 2 == 0) ? 'X' : 'O';
        printf("\n  %sPlayer %c will go first!%s\n", COLOR_GREEN, current_player, COLOR_RESET);
    }
    else if (mode == 2)
    {
        if (rand() % 2 == 0)
        {
            current_player = 'O'; // Computer goes first
            printf("\n  %sComputer will go first!%s\n", COLOR_GREEN, COLOR_RESET);
        }
        else
        {
            current_player = 'X'; // Human goes first
            printf("\n  %sYou go first!%s\n", COLOR_GREEN, COLOR_RESET);
        }
    }
    printf("\n  Game starts in: ");
    for (int i = 3; i > 0; i--)
    {
        printf("%s->%d%s", COLOR_YELLOW, i, COLOR_RESET);
        sleepms(1000);
    }
    printf("\n");

    while (1)
    {
        if (first_draw)
        {
            // Initial full screen draw only once
            clear_and_home();
            print_title();
            print_score();
            print_board();
            print_status(mode);
            first_draw = 0;
        }
        else
        {
            // Only update the changed cell and player (ultra fast!)
            update_cell_only(last_move);
            update_player_only(mode);
        }

        if (mode == 2 && current_player == 'O')
        {
            // Computer's turn
            show_message("Computer is thinking...", COLOR_YELLOW);
            sleep(2);
            move = computer_move();
            if (move != -1)
            {
                make_move(move);
            }
            clear_message_area();
        }
        else
        {
            // Human's turn
            char ch;
            int valid_input = 0;

            while (!valid_input)
            {
                CURSOR_POS(21, 1);
                printf(CLEAR_LINE);
                printf("  %sEnter position (1-9, or 0 to quit): %s", COLOR_GREEN, COLOR_RESET);
                ch = getc(stdin);

                // Consume the newline
                if (getc(stdin) != '\n')
                {
                    while (getc(stdin) != '\n')
                        ;
                }

                if (ch < '0' || ch > '9')
                {
                    show_message("[X] Invalid input! Please enter a number.", COLOR_RED);
                    sleep(1);
                    continue;
                }

                move = ch - '0';

                if (move == 0)
                {
                    show_message("Quitting game...", COLOR_YELLOW);
                    sleep(1);
                    return;
                }

                move--; // Convert to 0-indexed (1-9 becomes 0-8)

                if (!is_valid_move(move))
                {
                    show_message("[X] Position already taken! Try again.", COLOR_RED);
                    sleep(1);
                    continue;
                }

                valid_input = 1;
            }

            make_move(move);
            clear_message_area();
        }

        if (check_winner())
        {
            // Update score
            if (current_player == 'X')
            {
                tictactoe_score.x_wins++;
            }
            else
            {
                tictactoe_score.o_wins++;
            }

            update_score_only();
            update_cell_only(last_move);

            if (mode == 2 && current_player == 'O')
            {
                show_message("*** Computer wins! ***", COLOR_GREEN);
            }
            else
            {
                char msg[50];
                sprintf(msg, "*** Player %c wins! ***", current_player);
                show_message(msg, current_player == 'X' ? COLOR_RED : COLOR_GREEN);
            }
            sleep(3);
            break;
        }

        if (is_board_full())
        {
            tictactoe_score.ties++;

            update_score_only();
            update_cell_only(last_move);
            show_message("*** It's a tie! ***", COLOR_YELLOW);
            sleep(3);
            break;
        }

        // Switch player
        current_player = (current_player == 'X') ? 'O' : 'X';
    }
}

void print_help()
{
    printf("\n");
    printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("   %s%sTIC TAC TOE - HELP%s\n", COLOR_BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s================================%s\n\n", COLOR_CYAN, COLOR_RESET);
    
    printf("%sUSAGE:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  tictactoe              - Show interactive menu\n");
    printf("  tictactoe [mode]       - Start game directly\n");
    printf("  tictactoe --help       - Show this help\n\n");
    
    printf("%sMODES:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s1%s - Human vs Human    - Two players alternate turns\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s2%s - Human vs Computer - Play against smart AI\n\n", COLOR_GREEN, COLOR_RESET);
    
    printf("%sGAMEPLAY:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("   Board positions numbered 1-9:\n");
    printf("      %s1%s | %s2%s | %s3%s\n", COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
    printf("     -----------\n");
    printf("      %s4%s | %s5%s | %s6%s\n", COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
    printf("     -----------\n");
    printf("      %s7%s | %s8%s | %s9%s\n\n", COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
    // press enter to see more 
    printf("Press Enter to see more..!");
    getc(stdin);
    printf("  1 Player %sX%s is red, Player %sO%s is green\n", COLOR_RED, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
    printf("  2 Random player goes first each game\n");
    printf("  3 Enter 1-9 to place your mark\n");
    printf("  4 Enter 0 to quit current game\n");
    printf("  5 First to get 3 in a row wins\n");
    printf("  6 Score persists across multiple games\n\n");
    
    printf("%sEXAMPLES:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  tictactoe              # Interactive menu\n");
    printf("  tictactoe 1            # Quick 2-player game\n");
    printf("  tictactoe 2            # Play vs computer\n\n");
    
    printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("Developed by %sHirok Reza%s\n", COLOR_GREEN, COLOR_RESET);
    printf("CSE21, RUET\n");
    printf("%s================================%s\n\n", COLOR_CYAN, COLOR_RESET);

    printf("Press Enter to continue..!");
    getc(stdin);
}

shellcmd xsh_tictactoe(int32 nargs, char *args[])
{

    int choice;
    char ch;

    if (nargs == 1)
    {
        // No arguments, show menu
        while (1)
        {
            printf("\n");
            printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
            printf("%s    %sTIC TAC TOE GAME%s\n", COLOR_CYAN, COLOR_GREEN, COLOR_RESET);
            printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
            printf("%s1.%s Human vs Human\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s2.%s Human vs Computer\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s3.%s Help\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s4.%s Quit\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
            printf("%sEnter your choice: %s", COLOR_GREEN, COLOR_RESET);
            ch = getc(stdin);
            printf("%c\n", ch);
            choice = ch - '0';
            // Consume the newline
            if (getc(stdin) != '\n')
            {
                while (getc(stdin) != '\n')
                    ;
            }

            if (choice == 1)
            {
                play_game(1);
            }
            else if (choice == 2)
            {
                play_game(2);
            }
            else if (choice == 3)
            {
                print_help();
            }
            else if (choice == 4)
            {
                printf("\n%sThanks for playing!%s\n", COLOR_CYAN, COLOR_RESET);
                printf("Developed by %sHirok Reza%s, CSE21, RUET\n\n", COLOR_GREEN, COLOR_RESET);
                return 0;
            }
            else
            {
                printf("%sInvalid choice! Please try again.%s\n", COLOR_RED, COLOR_RESET);
            }

            printf("\n");
        }

        return 0;
    }
    else if (nargs == 2)
    {
        if ((strcmp(args[1], "--help") == 0) || (strcmp(args[1], "-h") == 0) || (strcmp(args[1], "help") == 0))
        {
            print_help();
            return 0;
        }

        // Validate that argument is a single digit 1 or 2
        if (args[1][0] < '1' || args[1][0] > '2' || args[1][1] != '\0')
        {
            printf("Error: Invalid mode '%s'\n", args[1]);
            printf("Valid modes are: 1 (Human vs Human) or 2 (Human vs Computer)\n");
            printf("Use 'tictactoe --help' for more information.\n");
            return 1;
        }

        choice = args[1][0] - '0';
        if (choice == 1)
        {
            printf("\n--- Human vs Human Mode ---\n");
            play_game(1);
            return 0;
        }
        else if (choice == 2)
        {
            printf("\n--- Human vs Computer Mode ---\n");
            printf("You are X, Computer is O\n");
            play_game(2);
            return 0;
        }
        else
        {
            printf("Invalid mode! Use 'tictactoe --help' for usage information.\n");
            return 1;
        }
    }
    else
    {
        printf("Too many arguments! Use 'tictactoe --help' for usage information.\n");
        return 1;
    }
}
