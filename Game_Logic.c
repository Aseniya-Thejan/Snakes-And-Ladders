#include "Maze.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


// To check is the Flag is in a valied spot
void validateFlag(int flag[3], int Maze[3][10][25]) {
    int f = flag[0];
    int r = flag[1];
    int c = flag[2];
    if (Maze[f][r][c] == 0 || Maze[f][r][c] == 7) {
        printf("Flag is placed in an invalid cell (floor=%d, row=%d, col=%d).\n\n",f, r, c);
        exit(1); // terminate program
    }
}

void Style(){
    printf("      ::::::::  ::::    :::     :::     :::    ::: :::::::::: ::::::::              :::     ::::    ::: :::::::::          :::            :::     :::::::::  :::::::::  :::::::::: :::::::::   :::::::: \n");
    printf("    :+:    :+: :+:+:   :+:   :+: :+:   :+:   :+:  :+:       :+:    :+:           :+: :+:   :+:+:   :+: :+:    :+:         :+:          :+: :+:   :+:    :+: :+:    :+: :+:        :+:    :+: :+:    :+: \n");
    printf("   +:+        :+:+:+  +:+  +:+   +:+  +:+  +:+   +:+       +:+                 +:+   +:+  :+:+:+  +:+ +:+    +:+         +:+         +:+   +:+  +:+    +:+ +:+    +:+ +:+        +:+    +:+ +:+         \n");
    printf("  +#++:++#++ +#+ +:+ +#+ +#++:++#++: +#++:++    +#++:++#  +#++:++#++         +#++:++#++: +#+ +:+ +#+ +#+    +:+         +#+        +#++:++#++: +#+    +:+ +#+    +:+ +#++:++#   +#++:++#:  +#++:++#++   \n");
    printf("        +#+ +#+  +#+#+# +#+     +#+ +#+  +#+   +#+              +#+         +#+     +#+ +#+  +#+#+# +#+    +#+         +#+        +#+     +#+ +#+    +#+ +#+    +#+ +#+        +#+    +#+        +#+    \n");
    printf("+#    #+# #+#   #+#+# #+#     #+# #+#   #+#  #+#       #+#    #+#         #+#     #+# #+#   #+#+# #+#    #+#         #+#        #+#     #+# #+#    #+# #+#    #+# #+#        #+#    #+# #+#    #+#     \n");
    printf("########  ###    #### ###     ### ###    ### ########## ########          ###     ### ###    #### #########          ########## ###     ### #########  #########  ########## ###    ###  ########       \n");
}

// parse [1,2,3] format into array for file handling
int parse_line(char line[], int arr[], int max) {
    int count = 0, num = 0, in_number = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] >= '0' && line[i] <= '9') {     //taking the current character
            num = num * 10 + (line[i] - '0');   //building the number
            in_number = 1;      // Mark that we are currently reading a number
        } else {
            if (in_number) {
                arr[count++] = num; // Store the completed number in the array
                num = 0;
                in_number = 0;
                if (count >= max){
                    break;
                }
            }
        }
    }
    return count;
}

void read_text_files(int walls[MAX_ROWS][MAX_COLS], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS], int flag[MAX_COLS], int *seed ) 
{
    FILE *f;
    char line[MAX_LINE];
    int rows;

    // Passing walls file to walls array
    rows = 0;
    f = fopen("walls.txt", "r");
    if (!f) {
        perror("walls.txt");
        exit(1);
    }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#'){    // Skip comment lines starting with #
            continue;
        }
        parse_line(line, walls[rows], MAX_COLS);    // parse line into walls array one by row
        rows++;         //row number of the walls array
        if (rows >= MAX_ROWS) {
            break;
        }
    }
    fclose(f);

    // Track how many stairs start or end from each cell
    int startCount[3][10][25] = {0};
    int endCount[3][10][25]   = {0};

    // Passing stairs file to stairs array
    rows = 0;
    f = fopen("stairs.txt", "r");
    if (!f) { 
        perror("stairs.txt"); 
        exit(1); 
    }
    while (fgets(line, sizeof(line), f)) {      // read one line
        if (line[0] == '#'){
            continue;
        }
        int temp[10];
        int col_count = parse_line(line, temp, MAX_COLS - 1); // parse line into temp. Keeping one space to add extra column later

        int startF = temp[0];       //starting and ending coordinates of the stairs
        int startR = temp[1];
        int startC = temp[2];
        int endF   = temp[3];
        int endR   = temp[4];
        int endC   = temp[5];

        // check the validity of the stairs
        if (startCount[startF][startR][startC] >= 2) {
            printf("Ignored stair: more than 2 stairs originate from cell (F:%d, R:%d, C:%d)\n", startF, startR, startC);
            continue;
        }
        if (endCount[endF][endR][endC] >= 2) {
            printf("Ignored stair: more than 2 stairs end at cell (F:%d, R:%d, C:%d)\n",endF, endR, endC);
            continue;
        }

        // If valied, copy them into stairs array
        for (int j = 0; j < 6; j++) {
            stairs[rows][j] = temp[j];
        }
        stairs[rows][6] = 1; // add extra column with value 1. Store the stair direction

        // update counters
        startCount[startF][startR][startC]++;
        endCount[endF][endR][endC]++;

        rows++;
        if (rows >= MAX_ROWS){      // maximum number of rows reached for stairs array
            break;
        }
    }
    fclose(f);


    // Paassing poles file to poles array
    rows = 0;
    f = fopen("poles.txt", "r");
    if (!f) {
        perror("poles.txt");
        exit(1);
    }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#'){    // Skip comment lines starting with #
            continue;
        }
        parse_line(line, poles[rows], MAX_COLS);    // parse line into poles array one by row
        rows++;
        if (rows >= MAX_ROWS){  // maximum number of rows reached for poles array
            break;
        }
    }
    fclose(f);

    // Passing flag file to flag array
    f = fopen("flag.txt", "r");
    if (!f) {
        perror("flag.txt");
        exit(1);
    }
    while (fgets(line, sizeof(line), f)) {  // read one line
        if (line[0] == '#') {
            continue;   // Skip comment lines starting with #
        }
        parse_line(line, flag, MAX_COLS);   // parse line into flag array
        break;
    }
    fclose(f);

    // Passing seed file to seed variable
    f = fopen("seed.txt", "r");
    if (!f) {
        perror("seed.txt");
        exit(1);
    }
    while (fgets(line, sizeof(line), f)) {  // read one line
        if (line[0] == '#') {
            continue;   // Skip comment lines starting with #
        }
        int temp[MAX_COLS]; // temporary array to hold parsed numbers
        parse_line(line, temp, MAX_COLS); // parse line into temp array
        *seed = temp[0];    // update the reference seed variable
        
        break;  // only read the first valid line
    }
    fclose(f);
}

void applywalls(int Maze[3][10][25], int walls[MAX_ROWS][MAX_COLS]) {
    FILE *f = fopen("walls.txt", "r");
    if (!f) {
        perror("walls.txt");    
        return;
    }

    char line[MAX_LINE];    
    int walls_rows = 0;

    while (fgets(line, sizeof(line), f)) {
        // Skip empty lines or comment lines starting with #
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        walls_rows++;       //counting the number of rows in walls file
    }

    fclose(f);

   for (int r = 0; r < walls_rows; r++) {
    int z  = walls[r][0];  // floor
    int x1 = walls[r][1];  // start row
    int y1 = walls[r][2];  // start column
    int x2 = walls[r][3];  // end row
    int y2 = walls[r][4];  // end column

    if (x1 == x2 && y1 != y2) {
        // same row from y1 to y2
        int start, end;
        if (y1 < y2) {
            start = y1;
            end = y2;
        } else {
            start = y2;
            end = y1;
        }

        for (int c = start; c <= end; c++) {
            Maze[z][x1][c] = 0;            // marking walls in the maze
        }

    } else if (y1 == y2 && x1 != x2) {
        // same column from x1 to x2
        int start, end;
        if (x1 < x2) {
            start = x1;
            end = x2;
        } else {
            start = x2;
            end = x1;
        }

        for (int row = start; row <= end; row++) {
            Maze[z][row][y1] = 0;         // marking walls in the maze
        }

    } 
        else if (x1 == x2 && y1 == y2) {
            // single cell wall
            Maze[z][x1][y1] = 0;       // marking wall in the maze
        } 
        else {
            // diagonal walls will be skipped
            printf("This diagonal wall record (%d) is skipped\n", r+1);
        }
    }

}

//To visualise the maze
void printMaze(int maze[3][10][25]) {
    for (int f = 0; f < 3; f++) {
        printf("Floor %d:\n", f);
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 25; c++) {
                printf("%d ", maze[f][r][c]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

// Randomize the maze according to the given distribution
void randomize_maze(int Maze[3][10][25]) {
    int floors[3 * 10 * 25];
    int rows[3 * 10 * 25];      // Arrays to hold the positions of 1 cells
    int cols[3 * 10 * 25];
    int count = 0;

    // Collect all "1" positions in the maze
    for (int f = 0; f < 3; f++) {
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 25; c++) {
                if (Maze[f][r][c] == 1) {
                    floors[count] = f;
                    rows[count] = r;
                    cols[count] = c;
                    count++;
                }
            }
        }
    }
    //the coordinates of all 1 cells are now stored in 3 arrays floors, rows, cols

    // Shuffle indices to randomize the order of elements in arrays
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        // swap floors
        int tmpF = floors[i];
        floors[i] = floors[j];
        floors[j] = tmpF;

        // swap rows
        int tmpR = rows[i];
        rows[i] = rows[j];
        rows[j] = tmpR;

        // swap cols
        int tmpC = cols[i];
        cols[i] = cols[j];
        cols[j] = tmpC;
    }
    //now the arrays floors, rows, cols contains the coordinates of all 1 cells in random order

    // Calculate counts for each category
    int n25a = count * 25 / 100; // 25% for 5
    int n35  = count * 35 / 100; // 35% for 1–4
    int n25b = count * 25 / 100; // 25% for 11–12
    int n10  = count * 10 / 100; // 10% for 13–15
    int n5   = count - (n25a + n35 + n25b + n10);
    int idx = 0;

    // 25% = 5      5 for 0 cost
    for (int k = 0; k < n25a; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 5;
    }

    // 35% = random {1,2,3,4}       costs at the player
    for (int k = 0; k < n35; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 1 + rand() % 4;
    }

    // 25% = random {11,12}     11 and 12 for +1 and +2 cost
    for (int k = 0; k < n25b; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 11 + rand() % 2;
    }

    // 10% = random {13,15}   13 and 15 for +3 and +5 cost
    for (int k = 0; k < n10; k++, idx++) {
        int r = rand() % 2;  // 0 or 1
        if (r == 0) {
            Maze[floors[idx]][rows[idx]][cols[idx]] = 13;
        } else {
            Maze[floors[idx]][rows[idx]][cols[idx]] = 15;
        }
    }

    // 5% = random {16,17}  16 and 17 for x2 and x3 the mp
    for (int k = 0; k < n5; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 16 + rand() % 2;
    }

    // Leftover restore to 1
    for (; idx < count; idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 1;
    }
}

// Initialize Bhavana cells on the first floor
void initialize_bhavana(int Maze[3][10][25]) {
    int rows_pos[4 * 3];    // Arrays to hold the positions of 12 bhavana cells
    int cols_pos[4 * 3];
    int count = 0;

    // Collect all cells on first floor with value 10. BHAWANA cells
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 25; c++) {
            if (Maze[0][r][c] == 10) {
                rows_pos[count] = r;
                cols_pos[count] = c;
                count++;
            }
        }
    }

    //Shuffle indices to randomize the order of elements in arrays
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        // Swap rows
        int tmpR = rows_pos[i];
        rows_pos[i] = rows_pos[j];
        rows_pos[j] = tmpR;

        // Swap cols
        int tmpC = cols_pos[i];
        cols_pos[i] = cols_pos[j];
        cols_pos[j] = tmpC;
    }
    //now the arrays floors, rows, cols contains the coordinates of all 1 cells in random order

    int idx = 0;

    // Assign values in 5 steps
    int step1 = 2; // 2 for 51
    int step2 = 2; // 2 for 52
    int step3 = 2; // 2 for 53
    int step4 = 2; // 2 for 54
    int step5 = count - (step1 + step2 + step3 + step4); // remaining 4 for 55

    for (int k = 0; k < step1; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 51; //foodpoisoned bhavana
    }
    for (int k = 0; k < step2; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 52; //disoriented bhavana
    }
    for (int k = 0; k < step3; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 53; //triggered bhavana
    }
    for (int k = 0; k < step4; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 54; //happy bhavana
    }
    for (int k = 0; k < step5; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 55; //normal bhavana
    }
}

// Block the cells who Cut stairs through floor 2 if they go from floor 1 to floor 3
void cut_stairs_through_floor2(int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int numStairs) {
    for (int s = 0; s < numStairs; s++) {   //For all stairs
        int startF = stairs[s][0];
        int startR = stairs[s][1];
        int startC = stairs[s][2];
        int endF   = stairs[s][3];
        int endR   = stairs[s][4];
        int endC   = stairs[s][5];

        // Only take stairs going from first floor to third
        if (startF == 0 && endF == 2) {
            // Floor 2 is index 1
     
            // Compute the exact midpoint of the stair line on floor 2
            float r_mid = (startR + endR) / 2.0f;
            float c_mid = (startC + endC) / 2.0f;

            // Find the lower and upper integer indices
            // This allows us to cover cells touched by edges or corners
            int r_floor = (int)floor(r_mid);
            int r_ceil  = (int)ceil(r_mid);     //use ceil and floor to take all cutting cells
            int c_floor = (int)floor(c_mid);
            int c_ceil  = (int)ceil(c_mid);

            // Loop through all affected cells on floor 2
            for (int r = r_floor; r <= r_ceil; r++) {
                for (int c = c_floor; c <= c_ceil; c++) {
                    Maze[1][r][c] = 0;  // Set intersected cell to 0
                }
            }
        }
    }
}

// Roll a dice from 1 to max
int rollDice(int max) {
    return (rand() % max) + 1;
}
// The function to return the direction in string format
const char* getDirection(int num) {
    if (num == 2) {
        return "North";
    } else if (num == 3) {      
        return "East";
    } else if (num == 4) {
        return "South";
    } else if (num == 5) {
        return "West";
    } else {
        return "Invalid input";
    }
}


Player rollMovementDice(Player p, int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int numrows, int poles[MAX_ROWS][MAX_COLS], int numpoles,int flag[MAX_COLS]) {
    // Roll movement dice and dimulate dise trow
    int movement = rollDice(6);
    p.movementDiceCount++;      // Increment movement dice count

    if (p.movementDiceCount % 4 != 0){
    printf("\nPlayer %c rolls and %d on the movement dice and moves %s by %d cells\n", p.name, movement, getDirection(p.movementDirection), movement);
    }       //if not a 4th movement dice roll, just print movement dice result

    // Every 4th movement dice roll, roll direction dice
    if (p.movementDiceCount % 4 == 0) {
        int dirDice = rollDice(6);

        //update the movement direction according to the direction dice
        switch (dirDice) {
            case 2: p.movementDirection = 2; break; // North
            case 3: p.movementDirection = 3; break; // East    
            case 4: p.movementDirection = 4; break; // South
            case 5: p.movementDirection = 5; break; // West
            case 1: break; // No change
            case 6: break; // No change
        }
        printf("\nPlayer %c rolls and %d on the movement dice and %d on the direction dice, changes direction to %s and moves %d cells\n", p.name, movement, dirDice, getDirection(p.movementDirection), movement);

        p.movementDiceCount = 0;    // Reset movement dice count
    }

    p = movePlayer(p, movement, Maze, stairs, poles, numpoles, numrows,flag);   //pass the values to the move player function

    return p;
}

// Function to return the no of stairs in the text file
int loadStairs(int stairs[MAX_ROWS][MAX_COLS]) {
    FILE *f = fopen("stairs.txt", "r");
    if (!f) {
        perror("stairs.txt");
        return 0;
    }

    char line[MAX_LINE];
    int numStairs = 0;

    while (fgets(line, sizeof(line), f)) {
        // Skip empty lines or comment lines starting with #
        if (line[0] == '#' || line[0] == '\n'){
            continue;
        }
        parse_line(line, stairs[numStairs], MAX_COLS);
        numStairs++;
        // Check if we have exceeded the maximum number of rows
        if (numStairs >= MAX_ROWS){
            break;
        } 
    }

    fclose(f);
    return numStairs;   // Return the number of stairs loaded
}

//function to return the no of poles in the text file
int loadpoles(int poles[MAX_ROWS][MAX_COLS]) {
    FILE *f = fopen("poles.txt", "r");
    if (!f) {
        perror("poles.txt");
        return 0;
    }

    char line[MAX_LINE];
    int numPoles = 0;

    while (fgets(line, sizeof(line), f)) {
        // Skip empty lines or comment lines starting with #
        if (line[0] == '#' || line[0] == '\n'){
            continue;
        }
        parse_line(line, poles[numPoles], MAX_COLS);
        numPoles++;
        // Check if we have exceeded the maximum number of rows
        if (numPoles >= MAX_ROWS){
            break;
        }
    }

    fclose(f);
    return numPoles;    // Return the number of poles loaded
}

//function to update the stair direction every STEPSIZE rounds
void updatestairdirection(int stairs[MAX_ROWS][MAX_COLS], int numRows, int rdn, Player p,int temp,int Maze[3][10][25],int poles[MAX_ROWS][MAX_COLS],int numPoles,int flag[MAX_COLS]) {
    // Change only once every STEPSIZE rounds
    if (rdn % STEPSIZE == 0) {
        for (int i = 0; i < numRows; i++) {     //1= from start to end, 2= from end to start
            // Randomize between 1 and 2
            stairs[i][6] = temp;
        }
        int dummy = 0;
        p = checkStairs(p, stairs, numRows, Maze, &dummy,flag); // if player is on the start of a stair cell move him
        p = checkpoles(p, poles, numPoles, Maze, &dummy);   // after travelling through a stair, if player is on a pole cell move him

    }
}

// Function to move the player through poles
Player checkpoles(Player p, int poles[MAX_ROWS][MAX_COLS], int numPoles, int Maze[3][10][25], int *poleFailed) {
    //loop through all poles
    for (int i = 0; i < numPoles; i++) {
        int topF = poles[i][0];
        int endF = poles[i][1];     //start and end floor of the pole
        int row = poles[i][2];
        int col = poles[i][3];

        // Check if player is anywhere on the pole 
        if (p.widthBlock == row && p.heightBlock == col && p.floor <= topF && p.floor > endF) {
            printf("Player %c lands on cell (F:%d, R:%d, C:%d) which is a pole cell. Player %c slides down and now placed at cell (F:%d, R:%d, C:%d) in floor %d.\n",p.name, p.floor, row, col, p.name, endF, row, col, endF);

            // Slide down from current floor down to endF
            for (int f = p.floor - 1; f >= endF; f--) {
                // Check all the cells in between top stair to end stair at the given coordinates
                int test=Maze[f][row][col];
                // If any of the cells in between is BLOCKED or starting area, send player back to starting point
                if (Maze[endF][row][col] == 0 || Maze[endF][row][col] == 7) {
                    if (Maze[endF][row][col] == 0){ //destination is blocked
                        printf("Stair destination (F:%d, R:%d, C:%d) is a BLOCKED cell! ",endF,row,col);
                    }
                    else{   //destination is starting area
                        printf("Stair destination (F:%d, R:%d, C:%d) is the starting area! ",endF,row,col);
                    }

                    printf("Player %c is sent back to his starting point.\n", p.name);

                    // Reset to starting area
                    if (p.name == 'A') {
                        p.floor=0; 
                        p.widthBlock=6; 
                        p.heightBlock=12; 
                    }
                    else if (p.name == 'B') { 
                        p.floor=0; 
                        p.widthBlock=9; 
                        p.heightBlock=8; 
                    }
                    else if (p.name == 'C') { 
                        p.floor=0; 
                        p.widthBlock=9; 
                        p.heightBlock=16; 
                    }

                    p.hasEntered = 0;   // mark player as not started
                    *poleFailed = 1;    // mark that pole action failed
                    return p;           // as advised movementpoints will not be deducted
                }

                p.floor = f; // move to intermediate floor
                int cellValue = Maze[endF][row][col];
                int x = 0;
                switch(cellValue) {
                    case 1:  p.movementPoints += -1; x=-1; break;
                    case 2:  p.movementPoints += -2; x=-2; break;
                    case 3:  p.movementPoints += -3; x=-3; break;
                    case 4:  p.movementPoints += -4; x=-4; break;
                    case 5:  p.movementPoints +=  0; x=0; break;
                    case 11: p.movementPoints +=  1; x=1; break;    //update movement points according to the destination cell value
                    case 12: p.movementPoints +=  2; x=2; break;
                    case 13: p.movementPoints +=  3; x=3; break;
                    case 15: p.movementPoints +=  5; x=5; break;
                    case 16: p.movementPoints *=  2; x=12; break;
                    case 17: p.movementPoints *=  3; x=13; break;
                }
                //printf(" | Cell value : %s | Movementpoints: %d\n",getCellDescription(cellValue),p.movementPoints);
                // Finally set player to the end of the pole
                p.floor      = endF;
                p.widthBlock = row;
                p.heightBlock = col;
                break; // only one pole per move
            }
        }
    }
    return p;   // return updated player positions
}

// Function to move the player through stairs
Player checkStairs(Player p, int stairs[MAX_ROWS][MAX_COLS], int numStairs, int Maze[3][10][25], int *stairFailed, int flag[3]) {
    int bestIndex = -1; // no stair found yet
    double bestDist = 1e9; // very large number

    // Loop through all stairs to find candidates
    for (int i = 0; i < numStairs; i++) {
        int type = stairs[i][6]; // 7th number: 1 or 2 to determine the stair direction
        // Extract stair coordinates based on type
        int startF, startR, startC;
        int endF, endR, endC;

        if (type == 1) {
            startF = stairs[i][0];
            startR = stairs[i][1];
            startC = stairs[i][2];
            endF   = stairs[i][3];
            endR   = stairs[i][4];
            endC   = stairs[i][5];
        } else if (type == 2) {
            endF   = stairs[i][0];
            endR   = stairs[i][1];
            endC   = stairs[i][2];
            startF = stairs[i][3];
            startR = stairs[i][4];
            startC = stairs[i][5];
        } else {
            continue; // invalid type
        }

        // Candidate stair?
        if (p.floor == startF && p.widthBlock == startR && p.heightBlock == startC) {
            // compute the distance to flag
            int df = flag[0] - endF;
            int dr = flag[1] - endR;
            int dc = flag[2] - endC;
            double dist = sqrt(df*df + dr*dr + dc*dc);  //from coods geometric distance formula

            if (dist == bestDist) {
                // if tie random choice
                if (rand() % 2 == 0) {
                    bestDist = dist;    //if the choice is 0, update bestDist and bestIndex
                    bestIndex = i;      //else keep the previous bestIndex
                }
            } else if (dist < bestDist) {
                bestDist = dist;        // new best distance
                bestIndex = i;          // update best index
            }
        }
    }

    if (bestIndex == -1) {
        // no stairs at this location
        return p;
    }

    // Use the best stair and continue
    int type = stairs[bestIndex][6];
    int startF, startR, startC;
    int endF, endR, endC;

    if (type == 1) {
        startF = stairs[bestIndex][0];
        startR = stairs[bestIndex][1];
        startC = stairs[bestIndex][2];
        endF   = stairs[bestIndex][3];
        endR   = stairs[bestIndex][4];
        endC   = stairs[bestIndex][5];
    } else {
        endF   = stairs[bestIndex][0];
        endR   = stairs[bestIndex][1];
        endC   = stairs[bestIndex][2];
        startF = stairs[bestIndex][3];
        startR = stairs[bestIndex][4];
        startC = stairs[bestIndex][5];
    }

    printf("Player %c lands on cell (F:%d, R:%d, C:%d) which is a stair cell. Player %c takes the stairs and now placed at cell (F:%d, R:%d, C:%d) in floor %d.\n",p.name, startF, startR, startC, p.name, endF, endR, endC, endF);

    // If stair ends on blocked or in strarting area, send player back to starting point
    if (Maze[endF][endR][endC] == 0 || Maze[endF][endR][endC] == 7) {
        if (Maze[endF][endR][endC] == 0){
            printf("Stair destination (F:%d, R:%d, C:%d) is a BLOCKED cell! ",endF,endR,endC);
        }
        else{
            printf("Stair destination (F:%d, R:%d, C:%d) is the starting area! ",endF,endR,endC);
        }

        printf("Player %c is sent back to his starting point.\n", p.name);

        // Reset to starting area
        if (p.name == 'A') { 
            p.floor=0; 
            p.widthBlock=6; 
            p.heightBlock=12; 
        }
        else if (p.name == 'B') { 
            p.floor=0; 
            p.widthBlock=9; 
            p.heightBlock=8; 
        }
        else if (p.name == 'C') { 
            p.floor=0; 
            p.widthBlock=9; 
            p.heightBlock=16; 
        }
        p.hasEntered = 0;   // mark player as not started
        *stairFailed = 1;   // mark that stair action failed
        return p;   // as advised movementpoints will not be deducted
    }

    // Otherwise apply effects of destination cell
    int cellValue = Maze[endF][endR][endC];
    int x = 0;
    switch(cellValue) {
         case 1:  p.movementPoints += -1; x=-1; break;
         case 2:  p.movementPoints += -2; x=-2; break;
         case 3:  p.movementPoints += -3; x=-3; break;
         case 4:  p.movementPoints += -4; x=-4; break;
         case 5:  p.movementPoints +=  0; x=0; break;
         case 11: p.movementPoints +=  1; x=1; break;       //update movement points according to the destination cell value
         case 12: p.movementPoints +=  2; x=2; break;
         case 13: p.movementPoints +=  3; x=3; break;
         case 15: p.movementPoints +=  5; x=5; break;
         case 16: p.movementPoints *=  2; x=12; break;  // x2 the mp
         case 17: p.movementPoints *=  3; x=13; break;  // x3 the mp
    }
    // printf(" | Cell value : %s | Movementpoints: %d\n",getCellDescription(cellValue),p.movementPoints);

    // Finally set player to the end of the stair
    p.floor      = endF;
    p.widthBlock = endR;
    p.heightBlock = endC;
    int dummy = 0;

    p = checkStairs(p, stairs, numStairs, Maze, &dummy,flag);   // if player is on the start of a stair cell again ,move him

    return p;
}

// Function to generate a random number in a given range
int randInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to return the cell type in string format
const char* getcelltype(int value) {
    switch (value) {
        case 51: return "Foodpoisoned";
        case 52: return "Disoriented";
        case 53: return "Triggered";
        case 54: return "Happy";
        case 55: return "Normal";
        default: return "Invalid value";
    }
}

// Function to move the player
Player movePlayer(Player p, int movement, int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS], int numpoles, int numrows, int flag[MAX_COLS]) {
    int oldRow = p.widthBlock;
    int oldCol = p.heightBlock;     //keep the old positions in case of failed move
    int oldFloor = p.floor; 

    int startRow = oldRow;
    int startCol = oldCol;
    int startFloor = oldFloor;
    int oldMovementPoints = p.movementPoints; 

    if(p.isTriggered==1){
        movement=movement*2;    // double the movement if triggered
        printf("Player %c is in trigger mode. So there will be %d movements. \n",p.name,movement);
    }

    //if the player is disoriented 
    if(p.disorientedRounds>0){
        p.movementDirection=randInRange(2,5);   //get a random direction
        p.disorientedRounds--;  //decrease the disoriented rounds by 1
        printf("Player %c is in Disoriented mode. So the direction will be overided to %d.\n",p.name,p.movementDirection);
        if(p.disorientedRounds==0){
            p.isDisoriented=0;  //if disoriented rounds is 0, set the flag to 0
            printf("%c has recovered from disorientation.\n",p.name);
        }
    }
    // Move step by step
    for (int step = 0; step < movement; step++) {
        int nextRow = oldRow;
        int nextCol = oldCol;
        int nextFloor = oldFloor;
        
        // act according to Direction
        switch(p.movementDirection) {
            case 2: nextRow -= 1; break; // North
            case 4: nextRow += 1; break; // South
            case 3: nextCol += 1; break; // East
            case 5: nextCol -= 1; break; // West
        }

        // check the Boundries
        if (nextRow < 0 || nextRow >= 10 || nextCol < 0 || nextCol >= 25) {
            printf("Player %c tried to move outside the maze! Staying at (F:%d, R:%d, C:%d)\n",p.name, p.floor, p.widthBlock, p.heightBlock);
            p.movementPoints = oldMovementPoints;   //if the move fails, reset the mp
            return p;
        }

        // Blocked/trap
        if (Maze[nextFloor][nextRow][nextCol] == 0 || Maze[nextFloor][nextRow][nextCol] == 7) {
            p.movementPoints = oldMovementPoints - 2;   //if the move fails, reset the mp and deduct 2 mp
            printf("Player %c encountered a Blocked cell at step %d (F:%d,R:%d,C:%d).)",p.name, step + 1, nextFloor, nextRow, nextCol); 
            printf("Cannot move in the %s direction. Player remains at (F:%d,R:%d,C:%d). -2 Movement points.\n", getDirection(p.movementDirection), p.floor, p.widthBlock, p.heightBlock);
            int cellValue2;
            // Check if movement points are depleted
            if (p.movementPoints <= 0) {
                int randRow = randInRange(7, 9);
                int randCol = randInRange(21, 24);  //choose a random cell in bhawana
                cellValue2 = Maze[0][randRow][randCol];
                p.floor = 0;
                p.movementPoints=0; //reset movement points to 0
                p.widthBlock = randRow;
                p.heightBlock = randCol;
                printf("Player %c movement points are depleted and requires replenishment. Transporting to Bawana.\n",p.name);
                printf("Player %c is placed on a %s cell and effects take place.\n", p.name, getcelltype(cellValue2));
        
                // Check Bhawana Special Cells
                if (cellValue2 >= 51 && cellValue2 <= 55) {
                    switch(cellValue2) {
                    
                        case 55:    //normal bhawana operations
                            int rad=randInRange(10, 100);   //get a rand number between 10 and 100
                            p.movementPoints += rad;
                            printf("Player %c eats from Bawana and earns %d movement points and is placed at the entrance of Bawana.\n", p.name, rad);
                            break;

                        case 54:    //happy bhawana operations
                            p.movementPoints += 200;    //add 200 movement points
                            printf("Player %c eats from Bawana and is happy. %c is placed at the entrance of Bawana with 200 movement points.\n", p.name, p.name);
                            break;

                        case 53:    //triggered bhawana operations
                            p.isTriggered = 1; // needs a flag in Player struct
                            p.movementPoints += 50;  //add 50 movement points
                            printf("Player %c eats from Bawana and is triggered due to bad quality of food. %c is placed at the entrance of Bawana with 50 movement points.\n", p.name, p.name);
                            break;

                        case 52:    //disoriented bhawana operations
                            p.isDisoriented = 1; // needs a flag in Player struct
                            p.disorientedRounds = 4;    // set disoriented rounds to 4
                            p.movementPoints += 50; //add 50 movement points
                            printf("Player %c eats from Bawana and is disoriented and is placed at the entrance of Bawana with 50 movement points.\n", p.name);
                            break;

                        case 51:    //foodpoisoned bhawana operations
                            p.isFoodPoisoned = 1; 
                            p.foodPoisonRounds = 3;
                            printf("Player %c eats from Bawana and have a bad case of food poisoning. Will need three rounds to recover.\n", p.name);
                            break;

                    }

                    // Send to entrance after effect
                    if(cellValue2!=51){
                        p.floor = 0;
                        p.widthBlock = 9;
                        p.heightBlock = 19;     //entrance of bhawana
                        p.movementDirection = 2;
                        p.movementDiceCount = 0;

                        int x;
                        //Movement Points Logic
                        int cellValue = Maze[p.floor][p.widthBlock][p.heightBlock]; // get the cell value of the entrance cell

                        switch(cellValue) {
                            case 1:  p.movementPoints += -1; x=-1; break;
                            case 2:  p.movementPoints += -2; x=-2; break;
                            case 3:  p.movementPoints += -3; x=-3; break;
                            case 4:  p.movementPoints += -4; x=-4; break;
                            case 5:  p.movementPoints +=  0; x=0; break;    //entrance cell value
                            case 11: p.movementPoints +=  1; x=1; break;
                            case 12: p.movementPoints +=  2; x=2; break;
                            case 13: p.movementPoints +=  3; x=3; break;
                            case 15: p.movementPoints +=  5; x=5; break;
                            case 16: p.movementPoints *=  2; x=12; break; // double points
                            case 17: p.movementPoints *=  3; x=13; break; // triple points

                        }
                        printf("Player %c sent to Bhawana entrance (F:0, R:9, C:19) | Cellvalue= %s | MovementPoints= %d\n", p.name, getCellDescription(cellValue), p.movementPoints);
                    }
                }
            }   
            return p;
        }

        // Commit the Move
        oldRow = nextRow;
        oldCol = nextCol;
        oldFloor = nextFloor;

        int cellValue = Maze[oldFloor][oldRow][oldCol]; // get the cell value of the new cell
        int x = 0;
        switch(cellValue) {
            case 1:  p.movementPoints += -1; x=-1; break;
            case 2:  p.movementPoints += -2; x=-2; break;
            case 3:  p.movementPoints += -3; x=-3; break;
            case 4:  p.movementPoints += -4; x=-4; break;
            case 5:  p.movementPoints +=  0; x=0; break;
            case 11: p.movementPoints +=  1; x=1; break;    //update movement points according to the destination cell value
            case 12: p.movementPoints +=  2; x=2; break;
            case 13: p.movementPoints +=  3; x=3; break;
            case 15: p.movementPoints +=  5; x=5; break;
            case 16: p.movementPoints *=  2; x=12; break;
            case 17: p.movementPoints *=  3; x=13; break;
        }

        // Print step info
        printf("     Step %d: Player %c is at (F:%d, R:%d, C:%d )| cellValue= %s | movementPoints=%d\n", step + 1, p.name, oldFloor, oldRow, oldCol, getCellDescription(cellValue), p.movementPoints);

        // exacute check poles and stairs after every step
        // Check Poles 

        int poleFailed = 0; // flag to indicate if pole action failed
        Player tempPole = p;    // temporary player to hold pole results
        tempPole.floor = oldFloor;
        tempPole.widthBlock = oldRow;   // update temp player position
        tempPole.heightBlock = oldCol;

        tempPole = checkpoles(tempPole, poles, numpoles, Maze, &poleFailed);    // if player is on a pole cell move him
        if (poleFailed) {
            
            return tempPole;    // if pole action failed return the temp player
        }

        oldFloor = tempPole.floor;
        oldRow = tempPole.widthBlock;   // update old position after if the pole action successsfull    
        oldCol = tempPole.heightBlock;

        // Check Stairs
        int stairFailed = 0;    // flag to indicate if stair action failed
        Player tempStair = tempPole;    // temporary player to hold stair results
        
        tempStair = checkStairs(tempStair, stairs, numrows, Maze, &stairFailed,flag);   // if player is on the start of a stair cell move him
        if (stairFailed) {
            return tempStair;   // if stair action failed return the temp player
        }

        oldFloor = tempStair.floor;
        oldRow = tempStair.widthBlock;  // update old position after if the stair action successsfull
        oldCol = tempStair.heightBlock;
        
    }

    // If movement points dropped below 0
    int cellValue2;
    if (p.movementPoints <= 0) {
        int randRow = randInRange(7, 9);    //random row and col in bhawana
        int randCol = randInRange(21, 24);  //random row and col in bhawana
        cellValue2 = Maze[0][randRow][randCol]; // get the cell value of the random bhawana cell
        p.floor = 0;
        p.movementPoints=0; //reset movement points to 0
        p.widthBlock = randRow;
        p.heightBlock = randCol;
        printf("Player %c movement points are depleted and requires replenishment. Transporting to Bawana.\n",p.name);
        printf("Player %c is placed on a %s cell and effects take place.\n", p.name, getcelltype(cellValue2));

        // Check Bhawana Special Cells
        if (cellValue2 >= 51 && cellValue2 <= 55) { //only bhawana cells
            switch(cellValue2) {
                
                case 55:    //normal bhawana operations
                    int rad=randInRange(10, 100);   //get a rand number between 10 and 100
                    p.movementPoints += rad;    //add the random number to movement points
                    printf("Player %c eats from Bawana and earns %d movement points and is placed at the entrance of Bawana.\n", p.name, rad);
                    break;  //break from the switch case

                case 54:    //happy bhawana operations
                    p.movementPoints += 200;    //add 200 movement points
                    printf("Player %c eats from Bawana and is happy. %c is placed at the entrance of Bawana with 200 movement points.\n", p.name, p.name);
                    break;

                case 53:    //triggered bhawana operations
                    p.isTriggered = 1; // needs a flag in Player struct
                    p.movementPoints += 50; //add 50 movement points
                    printf("Player %c eats from Bawana and is triggered due to bad quality of food. %c is placed at the entrance of Bawana with 50 movement points.\n", p.name, p.name);
                    break;
                    
                case 52:
                    p.isDisoriented = 1; // needs a flag in Player struct
                    p.disorientedRounds = 4;    // set disoriented rounds to 4
                    p.movementPoints += 50; //add 50 movement points
                    printf("Player %c eats from Bawana and is disoriented and is placed at the entrance of Bawana with 50 movement points.\n", p.name);
                    break;

                case 51:
                    p.isFoodPoisoned = 1;   // needs a flag in Player struct
                    p.foodPoisonRounds = 3;   // set foodpoisoned rounds to 3
                    printf("Player %c eats from Bawana and have a bad case of food poisoning. Will need three rounds to recover.\n", p.name);
                    break;

            }
            // Send to entrance after effect
            if(cellValue2!=51){
                p.floor = 0;
                p.widthBlock = 9;
                p.heightBlock = 19;    //entrance of bhawana
                p.movementDirection = 2;    //facing north
                p.movementDiceCount = 0;    //reset movement dice count

                int x;
                //Movement Points Logic
                int cellValue = Maze[p.floor][p.widthBlock][p.heightBlock]; // get the cell value of the entrance cell
                switch(cellValue) {
                    case 1:  p.movementPoints += -1; x=-1; break;
                    case 2:  p.movementPoints += -2; x=-2; break;
                    case 3:  p.movementPoints += -3; x=-3; break;
                    case 4:  p.movementPoints += -4; x=-4; break;
                    case 5:  p.movementPoints +=  0; x=0; break;    //entrance cell value
                    case 11: p.movementPoints +=  1; x=1; break;
                    case 12: p.movementPoints +=  2; x=2; break;
                    case 13: p.movementPoints +=  3; x=3; break;
                    case 15: p.movementPoints +=  5; x=5; break;
                    case 16: p.movementPoints *=  2; x=12; break; // double points
                    case 17: p.movementPoints *=  3; x=13; break; // triple points
                }
                printf("Player %c sent to Bhawana entrance (F:0, R:9, C:19) | Cellvalue=%s | MovementPoints=%d\n", p.name, getCellDescription(cellValue), p.movementPoints);
            }
        }  
    return p;
    } 

    // Commit move
    printf("And Player %c is now at floor= %d , row= %d , col= %d .\n", p.name,oldFloor, oldRow, oldCol);
    printf("Player %c moved %d cells that cost %d movement points and is left with %d movement points, and is moving in the %s direction.\n",p.name, movement, (oldMovementPoints - p.movementPoints), p.movementPoints, getDirection(p.movementDirection));

    p.floor = oldFloor;
    p.widthBlock = oldRow;
    p.heightBlock = oldCol;

    return p;
}


// Function to handle capturing logic without pointers
void checkCapture(int numPlayers, Player players[], int currentIndex, int entryFloor[], int entryRow[], int entryCol[], int Maze[3][10][25]) {
    // Loop through all players to check for capture
    for (int i = 0; i < numPlayers; i++) {
        // skip checking the current player itself
        if (i == currentIndex) {
            continue;
        }

        int f = players[currentIndex].floor;
        int r = players[currentIndex].widthBlock;   // current player's position
        int c = players[currentIndex].heightBlock;
        int blockValue = Maze[f][r][c]; // get the cell value of the current player's position

        if (blockValue >= 51 && blockValue <= 55) {
            continue; //no capturing in bhawana
        }

        // check if current player lands on another player's position
        if (players[i].hasEntered == 1 && players[i].floor == f && players[i].widthBlock == r && players[i].heightBlock == c) { 
            printf("Player %c is captured by %c and sent back to start!\n", players[i].name, players[currentIndex].name);
            players[i].hasEntered = 0;  // flag player as not started
            players[i].floor = entryFloor[i];
            players[i].widthBlock = entryRow[i];    // reset to starting position
            players[i].heightBlock = entryCol[i];
        }
    }
}

// Function to get direction as string
const char* checkDirection(int temp) {
    if (temp == 2) {
        return "Downwards";
    } else if (temp == 1) {
        return "Upwards";
    }
}

// Function to return the cell values in string format
const char* getCellDescription(int cellValue) {
    switch (cellValue) {
        case 1:  return "Cost 1";
        case 2:  return "Cost 2";
        case 3:  return "Cost 3";
        case 4:  return "Cost 4";
        case 5:  return "Neutral";
        case 11: return "Bonus +1 MP";
        case 12: return "Bonus +2 MP";
        case 13: return "Bonus +3 MP";
        case 15: return "Bonus +5 MP";
        case 16: return "Multiplier x2";
        case 17: return "Multiplier x3";
        default: return "Unknown cell";
    }
}

void Playgame(int walls[MAX_ROWS][MAX_COLS], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS], int Maze[3][10][25], Player players[3], int flag[MAX_COLS], int entryFloor[], int entryRow[], int entryCol[]) {
    int seed;
    int numrows=loadStairs(stairs); // Load stairs and get the number of rows
    int numpoles=loadpoles(poles);  // Load poles and get the number of poles
    read_text_files(walls, stairs, poles, flag, &seed); // Read configuration from text files
    applywalls(Maze, walls);    // Apply walls to the maze
    randomize_maze(Maze);   // Randomize the maze
    initialize_bhavana(Maze);   // Initialize Bhawana area
    cut_stairs_through_floor2(Maze, stairs, MAX_ROWS);  // recognize and block stairs in floor 2
   // printMaze(Maze);    // Print the maze for verification
    printf("The flag is at floor = %d, row = %d, col = %d\n", flag[0], flag[1], flag[2]);
    validateFlag(flag, Maze);   // Validate the flag position
    int rdn=0;
    int k=1;
    while (k) { // infinite loop until someone wins
        rdn++;  // round number
        printf("\n\n\nRound NO %d\n\n", rdn);
        int temp = (rand() % 2) + 1;
        if(rdn %STEPSIZE == 0){ // every 5 rounds randomize the stairs direction
             printf("Stairs directions randomized at round %d ! to %s \n", rdn, checkDirection(temp));
            updatestairdirection(stairs, numrows, rdn, players[0],temp,Maze,poles,numpoles,flag);
            updatestairdirection(stairs, numrows, rdn, players[1],temp,Maze,poles,numpoles,flag);
            updatestairdirection(stairs, numrows, rdn, players[2],temp,Maze,poles,numpoles,flag);

        }

        for (int i = 0; i < 3; i++) {   // for each player
            if (players[i].hasEntered == 0) {   // if player has not entered the maze
                int roll = rollDice(6);
                printf("\nPlayer %c is at the starting area and rolls %d on the movement dice", players[i].name, roll);
                if (roll == 6) {    // needs a 6 to enter
                    players[i].hasEntered = 1;
                    players[i].floor = entryFloor[i];
                    players[i].widthBlock  = entryRow[i];
                    players[i].heightBlock = entryCol[i];
                    int x;
                    // Movement Points Logic
                    int cellValue = Maze[players[i].floor][players[i].widthBlock][players[i].heightBlock];
                    switch(cellValue) {
                        case 1:  players[i].movementPoints += -1; x=-1; break;
                        case 2:  players[i].movementPoints += -2; x=-2; break;
                        case 3:  players[i].movementPoints += -3; x=-3; break;
                        case 4:  players[i].movementPoints += -4; x=-4; break;
                        case 5:  players[i].movementPoints +=  0; x=0; break;
                        case 11: players[i].movementPoints +=  1; x=1; break;   // entrance cell value mps
                        case 12: players[i].movementPoints +=  2; x=2; break;
                        case 13: players[i].movementPoints +=  3; x=3; break;
                        case 15: players[i].movementPoints +=  5; x=5; break;
                        case 16: players[i].movementPoints *=  2; x=12; break; // double points
                        case 17: players[i].movementPoints *=  3; x=13; break; // triple points
                    }
                    printf(" and is placed on cell (F:%d, R:%d, C:%d) of the maze. | cell type : %s | Mp : %d\n", players[i].floor,players[i].widthBlock,players[i].heightBlock,getCellDescription(cellValue),players[i].movementPoints);
                    
                    if(players[i].floor==flag[0] && players[i].widthBlock==flag[1] && players[i].heightBlock==flag[2]){ // check if player reached the flag
                        printf("\n\nPlayer %c reached the flag at floor = %d, row = %d, col = %d and won the game\n\n", players[i].name,players[i].floor,players[i].widthBlock,players[i].heightBlock);
                        k=0; // to break outer loop
                        break;
                    }  

                }else {
                    printf(" cannot enter the maze.\n");    // did not roll a 6
                }
                continue; // next player
            }
            if(players[i].hasEntered == 1){ // if player has entered the maze
                if(players[i].isFoodPoisoned==1){   // if player is food poisoned

                    players[i].foodPoisonRounds--;  // decrement food poison rounds
                    printf("Player %c is still food poisoned and misses the turn. %d more rounds to go\n", players[i].name, players[i].foodPoisonRounds);
                    
                    if(players[i].foodPoisonRounds==0){
                        players[i].isFoodPoisoned=0;
                        int cellValue2;
                        int randRow = randInRange(7, 9);
                        int randCol = randInRange(21, 24);
                        cellValue2 = Maze[0][randRow][randCol];
                        printf("Player %c is now fit to proceed from the food poisoning episode and now placed on a %s cell and the effects take place.\n", players[i].name, getcelltype(cellValue2));
                        
                        players[i].floor = 0;   
                        players[i].movementPoints=0;    //reset movement points to 0
                        players[i].widthBlock = randRow;    // send the player to bhawana
                        players[i].heightBlock = randCol;

                        //Check Bhawana Special Cells (51–55)
                        if (cellValue2 >= 51 && cellValue2 <= 55) {

                            switch(cellValue2) {  

                                case 55:    //normal bhawana operations
                                    int rad=randInRange(10, 100);   //get a rand number between 10 and 100
                                    players[i].movementPoints += rad;   //add the random number to movement points
                                    printf("Player %c eats from Bawana and earns %d movement points and is placed at the entrance of Bawana.\n", players[i].name, rad);
                                    break;

                                case 54:    //happy bhawana operations
                                    players[i].movementPoints += 200;   //add 200 movement points
                                    printf("Player %c eats from Bawana and is happy. %c is placed at the entrance of Bawana with 200 movement points.\n", players[i].name, players[i].name);
                                    break;

                                case 53:    //bad quality food
                                    players[i].isTriggered = 1; // needs a flag in Player struct
                                    players[i].movementPoints += 50;    //add 50 movement points
                                    printf("Player %c eats from Bawana and is triggered due to bad quality of food. %c is placed at the entrance of Bawana with 50 movement points.\n", players[i].name, players[i].name);
                                    break;

                                case 52:    //disoriented bhawana operations
                                    players[i].isDisoriented = 1; // needs a flag in Player struct
                                    players[i].disorientedRounds = 4;   // set disoriented rounds to 4
                                    players[i].movementPoints += 50;    //add 50 movement points
                                    printf("Player %c eats from Bawana and is disoriented and is placed at the entrance of Bawana with 50 movement points.\n", players[i].name);
                                    break;

                                case 51:
                                    players[i].isFoodPoisoned = 1;  // needs a flag in Player struct
                                    players[i].foodPoisonRounds = 3;    // set foodpoisoned rounds to 3
                                    printf("Player %c eats from Bawana and have a bad case of food poisoning. Will need three rounds to recover.\n", players[i].name);
                                    break;

                            }

                            // Send to entrance after effect
                            if(cellValue2!=51){
                                players[i].floor = 0;
                                players[i].widthBlock = 9;
                                players[i].heightBlock = 19;
                                players[i].movementDirection = 2;
                                players[i].movementDiceCount = 0;
                                
                                int x;
                                // Movement Points Logic
                                int cellValue = Maze[players[i].floor][players[i].widthBlock][players[i].heightBlock];  // get the cell value of the entrance cell
                                switch(cellValue) {
                                    case 1:  players[i].movementPoints += -1; x=-1; break;
                                    case 2:  players[i].movementPoints += -2; x=-2; break;
                                    case 3:  players[i].movementPoints += -3; x=-3; break;
                                    case 4:  players[i].movementPoints += -4; x=-4; break;
                                    case 5:  players[i].movementPoints +=  0; x=0; break;
                                    case 11: players[i].movementPoints +=  1; x=1; break;       // entrance cell value mps
                                    case 12: players[i].movementPoints +=  2; x=2; break;
                                    case 13: players[i].movementPoints +=  3; x=3; break;
                                    case 15: players[i].movementPoints +=  5; x=5; break;
                                    case 16: players[i].movementPoints *=  2; x=12; break; // double points
                                    case 17: players[i].movementPoints *=  3; x=13; break; // triple points
                                }
                                printf("Player %c sent to Bhawana entrance (F:0, R:9, C:19) | Cellvalue= %s | MovementPoints= %d\n", players[i].name, getCellDescription(x), players[i].movementPoints);
                            }
                        }  
                    }
                    continue;   // skip the turn
                }
                players[i] = rollMovementDice(players[i], Maze, stairs, numrows,poles,numpoles,flag);   // roll movement dice and move the player
                // Check Capture Logic
                checkCapture(3, players, i, entryFloor, entryRow, entryCol, Maze);

                if(players[i].movementPoints>250){
                    players[i].movementPoints = 250;    // cap movement points to 250
                    printf("Player %c has exceeded 250 movement points. So capping it to 250\n",players[i].name);
                }
                // Check Win Condition
                if(players[i].floor==flag[0] && players[i].widthBlock==flag[1] && players[i].heightBlock==flag[2]){
                    printf("\n\nPlayer %c reached the flag at floor= %d, row= %d, col= %d and won the game\n\n", players[i].name,players[i].floor,players[i].widthBlock,players[i].heightBlock);
                    k=0; // to break outer loop
                    break;
                }
                
            }
        }
    }
}

