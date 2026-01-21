

#include <stdio.h>
#include <stdlib.h>

void free_map(char **grid, int size) {
for (int i = 0; i < size; i++) 
    {	
        free(grid[i]);
    }
    free(grid);
}

int main() {
    int n;
    printf("Enter grid size (5-15): ");
    scanf("%d", &n);

    if (n < 5 || n > 15) 
	                                            // the limit is given in the assignment tute (5 to 15) size of grid
    {  
	printf("Invalid size. Defaulting to 5.\n");
        n = 5;
    }

                                                        // dynamic Allocation 
    char **grid = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)

    {	    
        grid[i] = (char *)malloc(n * sizeof(char));
    }

    
    for (int i = 0; i < n; i++) {          //starting with the dots useing loops here.. and also used increment and rela                                             tional operators
        for (int j = 0; j < n; j++) {
            grid[i][j] = '.';
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

    

    free_map(grid, n);
    return 0;
}
