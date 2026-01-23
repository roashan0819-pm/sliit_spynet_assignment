#include <stdio.h>
#include <stdlib.h>
#include <time.h>


                                // define a structure to hold player data
						    
typedef struct
{
	int x ,y;
	char symbol;
} 
        player;
                                                 // function to handle the items 
void initialize_map(char **grid, int n)
{
    srand(time(NULL));                             // Seed the random numbers

                                                // fill the empty spaces first
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = '.';
        }
    }

                                                     //  place items randomly
    int items_placed = 0;
    while (items_placed < 7)
    {
    int r = rand() % n;
    int c = rand() % n;

        if (grid[r][c] == '.')
	{ 
        if (items_placed < 3) grid[r][c] = 'I';      // 3 Intel
        else if (items_placed < 5) grid[r][c] = 'L'; // 2 Lives
        else if (items_placed < 6) grid[r][c] = '#'; // 1 Wall
        else grid[r][c] = 'X';                       // 1 Extraction
            
            items_placed++;
        }
    }
}

void display_map(char **grid, int n)  {
	system("clear");            //clear the linux terminal for the nice look
        printf("controls : W(up) ,A(left), S(Down), D(right), Q(quit)\n\n");
	for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {	
        printf("%c", grid[i][j]);
	}
        printf("\n");
}
}

void free_map(char **grid, int size) {
    for (int i = 0; i < size; i++) 
        free(grid);
    
    free(grid);
}

int main() {
    int n;
    printf("Enter grid size (5-15): ");
    scanf("%d", &n);

    if (n < 5 || n > 15) n = 5;               //the limit is given in the assignment tute (5 to 15) size of grid

                                
    char **grid = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        grid[i] = (char *)malloc(n * sizeof(char));
    }

                              // initialize Map with Items
    initialize_map(grid, n);

                            //set the player
    player p1 = {0, 0, '@'}; // start this top of the grid left side
    grid[p1.x][p1.y] = p1.symbol;

    char input;
    int running = 1;

    
    while (running) {
        display_map(grid, n);
        printf("\nYour Move: ");
        scanf(" %c", &input);            

        int old_x = p1.x;
        int old_y = p1.y;

        if (input == 'q' || input == 'Q') {
            running = 0;
            continue;
        }

                                                 // Calculate new position
        if (input == 'w' || input == 'W') p1.x--;
        else if (input == 's' || input == 'S') p1.x++;
        else if (input == 'a' || input == 'A') p1.y--;
        else if (input == 'd' || input == 'D') p1.y++;

        
        if (p1.x < 0 || p1.x >= n || p1.y < 0 || p1.y >= n) {
            printf("\nOut of bounds! Move rejected.\n");
            p1.x = old_x;        
            p1.y = old_y;
        } else {
                                  // Update grid
            grid[old_x][old_y] = '.'; 
            grid[p1.x][p1.y] = p1.symbol;
        }  
    	
    }



    
    free_map(grid, n);
    return 0;
}
