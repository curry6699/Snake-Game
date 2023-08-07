// Slide
//
// This program was written by Prithvi Sajit
//
//
// This program is based of the original arcade games Space Invaders and Galiga
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1
#define LASER_STRENGTH 4

// Prototype functions:
void array_values(int map[SIZE][SIZE], int array_row, int array_column,
int array_value);
void print_map(int map[SIZE][SIZE], int laser_y);
int move_laser(int map[SIZE][SIZE], int laser_y, int laser_updown);
void destroy_rocks(int map[SIZE][SIZE], int laser_y);
void tnt_block(int map[SIZE][SIZE], int laser_y, int column);
void shift_map(int map[SIZE][SIZE]);
int map_clear(int map[SIZE][SIZE]);
int stone_start(int map[SIZE][SIZE]);
int rotate_map(int map[SIZE][SIZE], int rotation);


int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};

    // This line creates our laser_y variable. The laser starts in the
    // middle of the map, at position 7.
    int laser_y = SIZE / 2;
    
    // This line asks for and scans in the number of blocks the user will input.
    int number_blocks;
    printf("How many blocks? ");
    scanf("%d", &number_blocks);
    
    int array_row;
    int array_column;
    int array_value;
    int i = 0;
    printf("Enter blocks: \n");
    
    // This line scans in valid values into the array.
    while (i < number_blocks) {
        scanf("%d %d %d", &array_row, &array_column, &array_value);      
        if (array_row >= 0 && array_column >= 0) {
            if (array_row < SIZE && array_column < SIZE) {
                
                // Function to scan in valid array values while avoiding nesting
                array_values(map, array_row, array_column, array_value);    
            }
        }
        i++;
    }
    
    print_map(map, laser_y);


    int instruction;
    int laser_updown;    
    int rotation;
    int win_condition  = 0;
    int lost_condition = 0;
    int rotation_loss = 0;

    // This line sets up a while loop to allow repeated user input.
    while (scanf("%d", &instruction) == 1) {      

        // This set of lines and functions allow the user to move the laser up 
        // and down.
        if (instruction == 1) {
            scanf("%d", &laser_updown);
            laser_y = move_laser(map, laser_y, laser_updown);            
            print_map(map, laser_y);                
        }
        
        // This set of lines and functions allow the user to destroy stones.        
        if (instruction == 2) {
            destroy_rocks(map, laser_y);    
            print_map(map, laser_y);
            win_condition = map_clear(map);
            
            // This line checks if the win condition is met.
            if (win_condition != 0) {
                printf ("Game Won! \n");
                return 0;           
            }   
        }
        // This set of lines and functions allows the user to shift the map left
        if (instruction == 3) {            
            lost_condition = stone_start(map);
            if (lost_condition == 0) {
                shift_map(map);
                print_map(map, laser_y);
            }
            
            // This line checks if there are rocks at [y][0] and accordingly
            // indicates if the user has lost.
            else if (lost_condition == 1) {
                print_map(map, laser_y);
                printf("Game Lost! \n");
                return 0;
            }

        }
        // This set of lines and functions allows the user to rotate the map 
        // once in either the clock-wise or anti-clockwise orientation.
        if (instruction == 4) {
            scanf("%d", &rotation);
            rotation_loss++;
            if (rotation_loss == 1) {
                rotate_map(map, rotation);
            }
            print_map(map, laser_y);
        }
    }

    return 0;
}




// All Functions used:

// Prints out the contents of the map array. 
// Also print out a > symbol to denote the current laser position.
void print_map(int map[SIZE][SIZE], int laser_y) {
    int i = 0;
    while (i < SIZE) {
        if (i == laser_y) {
            printf("> ");
        } else {
            printf("  ");
        }
        int j = 0;
        while (j < SIZE) {
            printf("%d ", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Function to scan in valid array values while avoiding nesting.
void array_values(int map[SIZE][SIZE], int array_row, int array_column,
int array_value) {
    // This line ensures valid values for TNT blocks.
    if (array_value <= 9 && array_value >= 4) {
        map[array_row][array_column] = array_value;
    }
                
    // This line ensures valid values for rocks.
    else if (array_value == 1) {
        map[array_row][array_column] = STONE;
    }
}

// Function to move laser.
// Returns the position of the laser as well.
int move_laser(int map[SIZE][SIZE], int laser_y, int laser_updown) {
    int output;
    if (laser_updown == 1 && laser_y < SIZE - 1 ) {
        output = laser_y + laser_updown;
    } 
    else if (laser_updown == -1 && laser_y > 0) {
        output = laser_y + laser_updown;
    }
    else {
        output = laser_y;
    }
    return output;
}

// Function to destroy stones that are on the laser's path.
// Destroys up to 4 stones.
void destroy_rocks(int map[SIZE][SIZE], int laser_y) {
    int destroyed_blocks = 0; 
    int column = 0; 
    while (column < SIZE && destroyed_blocks < LASER_STRENGTH) {
        if (map[laser_y][column] == STONE) {
            map[laser_y][column] = EMPTY;
            destroyed_blocks++;
        }
        else if (map[laser_y][column] >= 4 && map[laser_y][column] <= 9) {
            destroyed_blocks = LASER_STRENGTH;
            tnt_block(map, laser_y, column);    
        } 
        column++;
    }
}

// Function to emulate the tnt explosion.
void tnt_block(int map[SIZE][SIZE], int laser_y, int column ) {
    int radius = map[laser_y][column];   
    int row_i = 0; 
    while (row_i < SIZE) {
        int column_i = 0; 
        while (column_i < SIZE) {
            if (sqrt((column_i - column) * (column_i - column) + 
            (row_i - laser_y) * (row_i - laser_y)) < radius) {
                map[row_i][column_i] = EMPTY;
            }               
            else if (column_i == SIZE - 1) {
                row_i++;
            }
            column_i++;
        }
    }    
}



// Function to shift map leftwards by 1.
void shift_map(int map[SIZE][SIZE]) {    
    
    int column = 0; 
    while (column < SIZE - 1) {
        int row = 0;   
        while (row < SIZE) {
            map[row][column] = map[row][column + 1];
            map[row][column + 1] = EMPTY;
            row++;   
        }
        map[SIZE - 1][column] = EMPTY;    
        column++;
    }


}

// Function to check if the map is clear.
// This function returns values corresponding to win and loss.
int map_clear(int map[SIZE][SIZE]) {
    int row = 0; 
    int output = 1;
    while (row < SIZE) {
        int column = 0; 
        while (column < SIZE) {
            if (map[row][column] == EMPTY) {
                column++;                  
            }
            else if (map[row][column] != EMPTY) {
                output = 0;
                column = 15;
                row = 15;
            }
        }
        row++;
    }
    return output;
}

// Function to check if there are any stones on the start of the array.
// This function is used when the shift left instruction is used.
// This function returns whether the user loses due to this reason ^
int stone_start (int map[SIZE][SIZE]) {
    int output;
    int row = 0;
    while (row < SIZE) {
        if (map[row][0] != EMPTY) {
            row = SIZE;
            output = 1;
        }
        else {
            row++;
            output = 0;
        }
    }
    return output;
}


// Function to rotate the map by 90 degrees clockwise / anti-clockwise.
// This function returns a value when used, which allows it to only be used
// once as per the guidelines.
// The first part of the function stores all rotated values of the original 
// array.
int rotate_map(int map[SIZE][SIZE], int rotation) {
    int output = 1;
    int map_rotations[SIZE][SIZE] = {EMPTY};
    int column = 0;
    while (column < SIZE - 1) {
        int row = 0; 
        while (row < SIZE) {            
            if (map[row][column] == STONE && rotation == 1) {
                map_rotations[column][SIZE - 1 - row] = STONE;
                map[row][column] = EMPTY;
                          
            }
            else if (map[row][column] == STONE && rotation == 2) {
                map_rotations[SIZE - 1 - column][row] = STONE;
                map[row][column] = EMPTY;       
            }
            row++;                          
        }
        column++;  
    }
    
    // This part of the function feeds the original array all the shifted values
    // from the temporary 2nd array. 
    int column_i = 0;                   
    while (column_i < SIZE) { 
        int row_i = 0;     
        while (row_i < SIZE) {
            if (map_rotations[row_i][column_i] == STONE) {
                map[row_i][column_i] = STONE;

            }
            row_i++;
        }
        column_i++;
    }
    return output;
}

