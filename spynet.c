

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

                //  Game Constants
#define MIN_SIZE 5
#define MAX_SIZE 15
#define START_LIVES 3
#define REQUIRED_INTEL 3

               // Structure Definitions 
typedef struct {
    int x, y;
    int health;
    int intel;
    char symbol;
    int active; 
} Player;

                 // used Function Prototypes 
void initialize_map(char **grid, int n);
void display_map(char **grid, int n, Player players[], int p_count, int current);
void log_game_state(Player p, char input);
void free_map(char **grid, int n);

int main() {
    int n, p_count;

    
    system("clear");
    printf("--- Welcome to SpyNet: The Codebreaker Protocol ---\n");
    printf("Enter grid size (%d-%d): ", MIN_SIZE, MAX_SIZE);
    if (scanf("%d", &n) != 1 || n < MIN_SIZE || n > MAX_SIZE) n = 10;

    printf("Enter number of players (1-3): ");
    if (scanf("%d", &p_count) != 1 || p_count < 1 || p_count > 3) p_count = 1;

                     //  Dynamic Memory Allocation
    char **grid = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        grid[i] = (char *)malloc(n * sizeof(char));
    }

    initialize_map(grid, n);

                  // Initialize Player Array
    Player players[3];
    char syms[] = {'@', '&', '$'};
    for(int i = 0; i < p_count; i++) {
        players[i].x = i;         // starting positions
        players[i].y = 0;
        players[i].health = START_LIVES;
        players[i].intel = 0;
        players[i].symbol = syms[i];
        players[i].active = 1;
        grid[players[i].x][players[i].y] = players[i].symbol;
    }

            // Clear log file for new session
    FILE *f = fopen("gamelog.txt", "w");
    if(f) { fprintf(f, "--- NEW MISSION STARTED ---\n"); fclose(f); }

    int current = 0;
    int running = 1;

                   //  Main Game Loop
    while (running) {
        
        if (!players[current].active) {
            current = (current + 1) % p_count;
            continue;
        }

        display_map(grid, n, players, p_count, current);
        printf("\nAgent %c, Enter Move (W/A/S/D) or Q to Exit: ", players[current].symbol);
        
        char input;
        scanf(" %c", &input);

        if (input == 'q' || input == 'Q') {
            players[current].active = 0;
            grid[players[current].x][players[current].y] = '.';
        } else {
            int old_x = players[current].x;
            int old_y = players[current].y;

                               // Movement Processing
            if (input == 'w' || input == 'W') players[current].x--;
            else if (input == 's' || input == 'S') players[current].x++;
            else if (input == 'a' || input == 'A') players[current].y--;
            else if (input == 'd' || input == 'D') players[current].y++;

                        // Validation > Bounds, Walls, and Other Players..
            if (players[current].x < 0 || players[current].x >= n || 
                players[current].y < 0 || players[current].y >= n || 
                grid[players[current].x][players[current].y] == '#' ||
                grid[players[current].x][players[current].y] == '@' ||
                grid[players[current].x][players[current].y] == '&' ||
                grid[players[current].x][players[current].y] == '$') {
                
                players[current].health--;
                players[current].x = old_x; 
                players[current].y = old_y;
                printf("\a"); 
            } else {
                


		    char target = grid[players[current].x][players[current].y];
                
                                     // Item Collection Logic
                if (target == 'I') players[current].intel++;
                else if (target == 'L') players[current].health++;
                else if (target == 'X') {
                    if (players[current].intel >= REQUIRED_INTEL) {
                        display_map(grid, n, players, p_count, current);
                        printf("\n*** MISSION SUCCESS! Agent %c has escaped! ***\n", players[current].symbol);
                        running = 0;
                        break;
                    } else {
                        printf("\nExtraction Failed: Not enough Intel!\n");
                        players[current].active = 0;
                    }
                }
                
                                        // Update Grid Position
                grid[old_x][old_y] = '.';
                grid[players[current].x][players[current].y] = players[current].symbol;
            }
            log_game_state(players[current], input);
        }

                                       // Checking  Health
        if (players[current].health <= 0) {
            players[current].active = 0;
            grid[players[current].x][players[current].y] = '.';
        }

                                       // Check if anyone is still in the game
        int active_count = 0;
        for(int i = 0; i < p_count; i++) if(players[i].active) active_count++;
        
        if (active_count == 0) {
            printf("\n--- ALL AGENTS NEUTRALIZED. GAME OVER. ---\n");
            running = 0;
        }

        current = (current + 1) % p_count; 
    }

    
    free_map(grid, n);
    return 0;
}

              // Function Definitions

void initialize_map(char **grid, int n) {    //funtion to handle the items
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) grid[i][j] = '.';  // fill the empty spaces first

    int items = 0;                                   //place items randomly
    while (items < 12) { 
        int r = rand() % n, c = rand() % n;
        if (grid[r][c] == '.' && !(r < 3 && c == 0)) {
            if (items < 5) grid[r][c] = 'I';      // Intel
            else if (items < 8) grid[r][c] = 'L'; // Lives
            else if (items < 11) grid[r][c] = '#'; // Walls
            else grid[r][c] = 'X';                // Extraction
            items++;
        }
    }
}

void display_map(char **grid, int n, Player players[], int p_count, int current) {
    system("clear");
    printf("========================================\n");
    printf("   SPYNET: THE CODEBREAKER PROTOCOL     \n");
    printf("========================================\n");
    for(int i = 0; i < p_count; i++) {
        if(players[i].active) {
            printf("Agent %c | Lives: %d | Intel: %d/3 %s\n", 
                players[i].symbol, players[i].health, players[i].intel, 
                (i == current ? "<< CURRENT TURN >>" : ""));
        }
    }
    printf("----------------------------------------\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%c ", grid[i][j]);
        printf("\n");
    }
}

void log_game_state(Player p, char input) {
    FILE *fptr = fopen("gamelog.txt", "a");
    if (fptr) {
        fprintf(fptr, "Agent %c: Action[%c] Pos(%d,%d) HP:%d Intel:%d\n", 
                p.symbol, input, p.x, p.y, p.health, p.intel);
        fclose(fptr);
    }
}

void free_map(char **grid, int n) {
    for (int i = 0; i < n; i++) free(grid[i]);
    free(grid);
}
