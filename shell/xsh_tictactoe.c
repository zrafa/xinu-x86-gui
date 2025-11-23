#include <xinu.h>
#include <stdio.h>

char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
char current_player = 'X';

void print_board() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0], board[1], board[2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[3], board[4], board[5]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[6], board[7], board[8]);
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

void reset_board() {
    for (int i = 0; i < 9; i++) {
        board[i] = '1' + i;
    }
    current_player = 'X';
}

void play_game(int mode) {
    reset_board();
    int move;
    
    while (1) {
        print_board();
        
        if (mode == 2 && current_player == 'O') {
            // Computer's turn
            printf("Computer's turn (O)...\n");
            printf("Thinking...\n");
            sleep(1);
            move = computer_move();
            if (move != -1) {
                make_move(move);
            }
        } else {
            // Human's turn
            char ch;
            printf("Player %c's turn. Enter position (1-9, or 0 to quit): ", current_player);
            
            ch = getc(stdin);
            printf("%c\n", ch);
            
            // Consume the newline
            if (getc(stdin) != '\n') {
                while (getc(stdin) != '\n');
            }
            
            if (ch < '0' || ch > '9') {
                printf("Invalid input! Please enter a number.\n");
                continue;
            }
            
            move = ch - '0';
            
            if (move == 0) {
                printf("Player %c quits the game. Exiting...\n", current_player);
                return;
            }
            move--;  // Convert to 0-indexed
            
            if (!is_valid_move(move)) {
                printf("Invalid move! Try again.\n");
                continue;
            }
            make_move(move);
        }
        
        if (check_winner()) {
            print_board();
            if (mode == 2 && current_player == 'O') {
                printf("Computer wins!\n");
            } else {
                printf("Player %c wins!\n", current_player);
            }
            break;
        }
        
        if (is_board_full()) {
            print_board();
            printf("It's a tie!\n");
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
        printf("================================\n");
        printf("     TIC TAC TOE GAME\n");
        printf("================================\n");
        printf("1. Human vs Human\n");
        printf("2. Human vs Computer\n");
        printf("3. Quit\n");
        printf("================================\n");
        printf("Enter your choice: ");
        ch = getc(stdin);
        choice = ch - '0';
        printf("%c\n", ch);
        // Consume the newline
        if (getc(stdin) != '\n') {
            while (getc(stdin) != '\n');
        }
        
        if (choice == 1) {
            printf("\n--- Human vs Human Mode ---\n");
            play_game(1);
        } else if (choice == 2) {
            printf("\n--- Human vs Computer Mode ---\n");
            printf("You are X, Computer is O\n");
            play_game(2);
        } else if (choice == 3) {
            printf("Thanks for playing!\n");
            return 0;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
        
        printf("\n");
    }
    
    return 0;
}
