#include <xinu.h>
#include <stdio.h>

// ANSI escape codes for cursor control and colors
#define CLEAR_SCREEN "\033[2J"
#define CURSOR_HOME "\033[H"
#define COLOR_RED     "\033[31m"  // bright red for X
// #define COLOR_BLUE    "\033[34m"   // it look bad on black screen
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"

char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
char current_player = 'X';
int last_move = -1;

typedef struct {
    int x_wins;
    int o_wins;
    int ties;
} TicTacToeScore;

TicTacToeScore tictactoe_score = {0, 0, 0};

const char* get_cell_color(int pos) {
    if (board[pos] == 'X') return COLOR_RED;
    if (board[pos] == 'O') return COLOR_GREEN;
    return COLOR_YELLOW;  // Available positions
}

void print_board() {
    printf("\n");
    printf("  %s+---+---+---+%s\n", COLOR_CYAN, COLOR_RESET);
    
    for (int row = 0; row < 3; row++) {
        printf("  %s|%s", COLOR_CYAN, COLOR_RESET);
        for (int col = 0; col < 3; col++) {
            int pos = row * 3 + col;
            const char* color = get_cell_color(pos);
            const char* bold = (pos == last_move) ? COLOR_BOLD : "";
            printf(" %s%s%c%s %s|%s", bold, color, board[pos], COLOR_RESET, COLOR_CYAN, COLOR_RESET);
        }
        printf("\n");
        
        if (row < 2) {
            printf("  %s+---+---+---+%s\n", COLOR_CYAN, COLOR_RESET);
        }
    }
    
    printf("  %s+---+---+---+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("\n");
}

int check_winner() {
    int win_conditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
        {0, 4, 8}, {2, 4, 6}              // diagonals
    };
    
    for (int i = 0; i < 8; i++) {
        if (board[win_conditions[i][0]] == board[win_conditions[i][1]] &&
            board[win_conditions[i][1]] == board[win_conditions[i][2]]) {
            return 1;
        }
    }
    return 0;
}

int is_board_full() {
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O') {
            return 0;
        }
    }
    return 1;
}

int is_valid_move(int position) {
    if (position < 0 || position > 8) {
        return 0;
    }
    return (board[position] != 'X' && board[position] != 'O');
}

void make_move(int position) {
    board[position] = current_player;
    last_move = position;
}

void print_score() {
    printf("  %s+======== SCORE ===========+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s|%s  %sX: %2d%s | %sO: %2d%s | Tie: %2d %s|%s\n", 
           COLOR_CYAN, COLOR_RESET,
           COLOR_RED, tictactoe_score.x_wins, COLOR_RESET,
           COLOR_GREEN, tictactoe_score.o_wins, COLOR_RESET,
           tictactoe_score.ties,
           COLOR_CYAN, COLOR_RESET);
    printf("  %s+==========================+%s\n", COLOR_CYAN, COLOR_RESET);
}

void print_status(int mode) {
    printf("\n  %s===============================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  Current Player: %s%s%c%s  |  Mode: %s\n",
           COLOR_BOLD,
           current_player=='X'?COLOR_RED:COLOR_GREEN,
           current_player,
           COLOR_RESET,
           mode==1?"Human vs Human":"Human vs Computer");
    printf("  %s===============================================%s\n", COLOR_CYAN, COLOR_RESET);
}

typedef struct {
    int position;
    int score;
} Move;

int get_available_count() {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (is_valid_move(i)) {
            count++;
        }
    }
    return count;
}

Move minimax(char player, char max_player) {
    Move best_move;
    char other_player = (player == 'X') ? 'O' : 'X';
    
    // Check if previous move was a winner
    if (current_player == 'X') {
        current_player = 'O';
    } else {
        current_player = 'X';
    }
    
    if (check_winner()) {
        current_player = player;
        char winner = other_player;
        int score = (winner == max_player) ? (get_available_count() + 1) : -(get_available_count() + 1);
        best_move.position = -1;
        best_move.score = score;
        return best_move;
    }
    
    current_player = player;
    
    // Check for tie
    if (is_board_full()) {
        best_move.position = -1;
        best_move.score = 0;
        return best_move;
    }
    
    // Initialize best move
    if (player == max_player) {
        best_move.score = -99999;  // Maximize
    } else {
        best_move.score = 99999;   // Minimize
    }
    best_move.position = -1;
    
    // Try all possible moves
    for (int i = 0; i < 9; i++) {
        if (is_valid_move(i)) {
            char original = board[i];
            char original_winner = current_player;
            
            // Make move
            board[i] = player;
            if (check_winner()) {
                current_player = player;
            }
            
            // Simulate game with minimax
            Move sim_move = minimax(other_player, max_player);
            sim_move.position = i;
            
            // Undo move
            board[i] = original;
            current_player = original_winner;
            
            // Update best move
            if (player == max_player) {
                if (sim_move.score > best_move.score) {
                    best_move = sim_move;
                }
            } else {
                if (sim_move.score < best_move.score) {
                    best_move = sim_move;
                }
            }
        }
    }
    
    return best_move;
}

int computer_move() {
    // Use minimax algorithm for smart AI
    if (get_available_count() == 9) {
        // First move: choose randomly
        int available[9];
        int count = 0;
        for (int i = 0; i < 9; i++) {
            if (is_valid_move(i)) {
                available[count++] = i;
            }
        }
        return available[rand() % count];
    }
    
    // Use minimax for subsequent moves
    Move best_move = minimax('O', 'O');
    return best_move.position;
}

void clear_and_home() {
    printf(CLEAR_SCREEN);
    printf(CURSOR_HOME);
}

void reset_board() {
    for (int i = 0; i < 9; i++) {
        board[i] = '1' + i;
    }
    current_player = 'X';
    last_move = -1;
}

void print_title() {
    printf("\n");
    printf("  %s+===============================+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("  %s|%s       %s%sTIC TAC TOE GAME%s         %s|%s\n", 
           COLOR_CYAN, COLOR_RESET, COLOR_BOLD, COLOR_GREEN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("  %s+===============================+%s\n", COLOR_CYAN, COLOR_RESET);
}

void play_game(int mode) {
    reset_board();
    int move;
    
    // In mode 2, randomly decide who goes first
    if (mode == 2) {
        clear_and_home();
        print_title();
        if (rand() % 2 == 0) {
            current_player = 'O';  // Computer goes first
            printf("\n  %sComputer will go first!%s\n", COLOR_GREEN, COLOR_RESET);
        } else {
            printf("\n  %sYou go first!%s\n", COLOR_GREEN, COLOR_RESET);
        }
        printf("\n  Game starts in: ");
        for(int i=3 ; i>0;i--){
            printf("%s%d%s ", COLOR_YELLOW, i, COLOR_RESET);
            sleep(1);
        }
        printf("\n");
    }
    
    while (1) {
        // Clear and redraw at same position
        clear_and_home();
        print_title();
        print_score();
        print_board();
        print_status(mode);
        
        if (mode == 2 && current_player == 'O') {
            // Computer's turn
            printf("\n  %sComputer is thinking%s", COLOR_YELLOW, COLOR_RESET);
            for (int i = 0; i < 3; i++) {
                printf(".");
                sleep(1);
            }
            printf("\n");
            move = computer_move();
            if (move != -1) {
                make_move(move);
            }
        } else {
            // Human's turn
            char ch;
            int valid_input = 0;
            
            while (!valid_input) {
                printf("\n  %sEnter position (1-9, or 0 to quit): %s", COLOR_GREEN, COLOR_RESET);
                ch = getc(stdin);
                printf("%c\n", ch);
                
                // Consume the newline
                if (getc(stdin) != '\n') {
                    while (getc(stdin) != '\n');
                }
                
                if (ch < '0' || ch > '9') {
                    clear_and_home();
                    print_title();
                    print_score();
                    print_board();
                    print_status(mode);
                    printf("\n  %s[X] Invalid input! Please enter a number.%s\n", COLOR_RED, COLOR_RESET);
                    sleep(1);
                    continue;
                }
                
                move = ch - '0';
                
                if (move == 0) {
                    printf("\n  %sPlayer %c quits the game. Exiting...%s\n", COLOR_YELLOW, current_player, COLOR_RESET);
                    sleep(1);
                    return;
                }
                
                move--;  // Convert to 0-indexed (1-9 becomes 0-8)
                
                if (!is_valid_move(move)) {
                    clear_and_home();
                    print_title();
                    print_score();
                    print_board();
                    print_status(mode);
                    printf("\n  %s[X] Position already taken! Try again.%s\n", COLOR_RED, COLOR_RESET);
                    sleep(1);
                    continue;
                }
                
                valid_input = 1;
            }
            
            make_move(move);
        }
        
        if (check_winner()) {
            clear_and_home();
            print_title();
            
            // Update score
            if (current_player == 'X') {
                tictactoe_score.x_wins++;
            } else {
                tictactoe_score.o_wins++;
            }
            
            print_score();
            print_board();
            
            if (mode == 2 && current_player == 'O') {
                printf("\n  %s*** Computer wins! ***%s\n", COLOR_GREEN, COLOR_RESET);
            } else {
                printf("\n  %s*** Player %c wins! ***%s\n", 
                       current_player=='X'?COLOR_RED:COLOR_GREEN, 
                       current_player, 
                       COLOR_RESET);
            }
            sleep(3);
            break;
        }
        
        if (is_board_full()) {
            clear_and_home();
            print_title();
            
            tictactoe_score.ties++;
            
            print_score();
            print_board();
            printf("\n  %s*** It's a tie! ***%s\n", COLOR_YELLOW, COLOR_RESET);
            sleep(3);
            break;
        }
        
        // Switch player
        current_player = (current_player == 'X') ? 'O' : 'X';
    }
}

void print_help() {
    printf("\n");
    printf("TIC TAC TOE - How to Play\n");
    printf("=========================\n\n");
    printf("USAGE:\n");
    printf("  tictactoe [mode]         - Start a new game\n");
    printf("  tictactoe                - Show Main Menu\n");
    printf("  tictactoe help           - Show this help message\n\n");
    printf("MODES:\n");
    printf("  1 - Human vs Human       - Two players take turns\n");
    printf("  2 - Human vs Computer    - Play against AI\n\n");
    printf("GAMEPLAY:\n");
    printf("  - Board positions are numbered 1-9:\n");
    printf("      1 | 2 | 3\n");
    printf("     ---|---|---\n");
    printf("      4 | 5 | 6\n");
    printf("     ---|---|---\n");
    printf("      7 | 8 | 9\n\n");
    printf("  - Enter position number (1-9) to place your mark\n");
    printf("  - Enter 0 to quit the current game\n");
    printf("  - First to get 3 in a row wins!\n\n");
    printf("EXAMPLES:\n");
    printf("  tictactoe 1              - Start human vs human\n");
    printf("  tictactoe 2              - Start human vs computer\n\n");
}

shellcmd xsh_tictactoe(int32 nargs, char *args[]) {

    int choice;
    char ch;
    if(nargs == 1) {
        // No arguments, show menu
    } else if(nargs == 2 ) {
        if ((strcmp(args[1], "--help") == 0) || (strcmp(args[1], "-h") == 0) || (strcmp(args[1], "help") == 0)) {
            print_help();
            return 0;
        }
                
        // Validate that argument is a single digit 1 or 2
        if (args[1][0] < '1' || args[1][0] > '2' || args[1][1] != '\0') {
            printf("Error: Invalid mode '%s'\n", args[1]);
            printf("Valid modes are: 1 (Human vs Human) or 2 (Human vs Computer)\n");
            printf("Use 'tictactoe --help' for more information.\n");
            return 1;
        }
        
        choice = args[1][0] - '0';
        if (choice == 1) {
            printf("\n--- Human vs Human Mode ---\n");
            play_game(1);
            return 0;
        } else if (choice == 2) {
            printf("\n--- Human vs Computer Mode ---\n");
            printf("You are X, Computer is O\n");
            play_game(2);
            return 0;
        } else {
            printf("Invalid mode! Use 'tictactoe --help' for usage information.\n");
            return 1;
        }
    } else {
        printf("Too many arguments! Use 'tictactoe --help' for usage information.\n");
        return 1;
    }
    
    while (1) {
        printf("\n");
        printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s    %sTIC TAC TOE GAME%s\n", COLOR_CYAN, COLOR_GREEN, COLOR_RESET);
        printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s1.%s Human vs Human\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s2.%s Human vs Computer\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s3.%s Quit\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%sEnter your choice: %s", COLOR_GREEN, COLOR_RESET);
        ch = getc(stdin);
        printf("%c\n", ch);
        choice = ch - '0';
        // Consume the newline
        if (getc(stdin) != '\n') {
            while (getc(stdin) != '\n');
        }
        
        if (choice == 1) {
            printf("\n%s--- Human vs Human Mode ---%s\n", COLOR_GREEN, COLOR_RESET);
            play_game(1);
        } else if (choice == 2) {
            printf("\n%s--- Human vs Computer Mode ---%s\n", COLOR_GREEN, COLOR_RESET);
            printf("You are %sX%s, Computer is %sO%s\n", COLOR_RED, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
            play_game(2);
        } else if (choice == 3) {
            printf("%sThanks for playing!%s\n", COLOR_CYAN, COLOR_RESET);
            return 0;
        } else {
            printf("%sInvalid choice! Please try again.%s\n", COLOR_RED, COLOR_RESET);
        }
        
        printf("\n");
    }
    
    return 0;
}
