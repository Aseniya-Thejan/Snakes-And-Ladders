#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_LINE 100     // maximum characters in one line
#define MAX_ROWS 20      // maximum number of rows per file
#define MAX_COLS 20      // maximum numbers per row

typedef struct {
        char name;       // Player name: 'A', 'B', 'C'
        int floor;       // Current floor (0 = first floor, 1 = second, etc.)
        int widthBlock;  // Current position in width (column) in blocks
        int lengthBlock; // Current position in length (row) in blocks
        int movementDirection; // 2=north, 3=east, 4=south, 5=west
        int movePoints;  // Remaining movement points
        int movementDiceCount;
        int roundNo;
    } Player;

// Helper: parse [1,2,3] format into array
int parse_line(char line[], int arr[], int max) {
    int count = 0, num = 0, in_number = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            num = num * 10 + (line[i] - '0');
            in_number = 1;
        } else {
            if (in_number) {
                arr[count++] = num;
                num = 0;
                in_number = 0;
                if (count >= max) break;
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

    // --- Walls ---
    rows = 0;
    f = fopen("walls.txt", "r");
    if (!f) { perror("walls.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;
        parse_line(line, walls[rows], MAX_COLS);
        rows++;
        if (rows >= MAX_ROWS) break;
    }
    fclose(f);

    // --- Stairs ---
    rows = 0;
    f = fopen("stairs.txt", "r");
    if (!f) { perror("stairs.txt"); exit(1); }

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;

        int col_count = parse_line(line, stairs[rows], MAX_COLS - 1); // leave space for extra column
        stairs[rows][col_count] = 1;  // add extra column with value 1
        rows++;

        if (rows >= MAX_ROWS) break;
    }
    fclose(f);

    // --- Poles ---
    rows = 0;
    f = fopen("poles.txt", "r");
    if (!f) { perror("poles.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;
        parse_line(line, poles[rows], MAX_COLS);
        rows++;
        if (rows >= MAX_ROWS) break;
    }
    fclose(f);

    // --- Flag (only first valid line) ---
    f = fopen("flag.txt", "r");
    if (!f) { perror("flag.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;
        parse_line(line, flag, MAX_COLS);
        break;
    }
    fclose(f);

    // --- Seed (only first valid value) ---
    f = fopen("seed.txt", "r");
    if (!f) { perror("seed.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;
        int temp[MAX_COLS];
        if (parse_line(line, temp, MAX_COLS) > 0) {
            *seed = temp[0]; // update caller’s seed
        }
        break;
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
        walls_rows++;
    }

    fclose(f);

   for (int r = 0; r < walls_rows; r++) {
    int z  = walls[r][0];  // floor
    int x1 = walls[r][1];  // start row
    int y1 = walls[r][2];  // start column
    int x2 = walls[r][3];  // end row
    int y2 = walls[r][4];  // end column

    if (x1 == x2 && y1 != y2) {
        // same row → zero columns from y1 to y2
        int start, end;
        if (y1 < y2) {
            start = y1;
            end = y2;
        } else {
            start = y2;
            end = y1;
        }

        for (int c = start; c <= end; c++) {
            Maze[z][x1][c] = 0;
        }
        } 
        else if (y1 == y2 && x1 != x2) {
            // same column → zero rows from x1 to x2
            int start, end;
            if (x1 < x2) {
                start = x1;
                end = x2;
            } else {
                start = x2;
                end = x1;
            }

            for (int row = start; row <= end; row++) {
                Maze[z][row][y1] = 0;
            }
        } 
        else if (x1 == x2 && y1 == y2) {
            // single cell → zero that cell
            Maze[z][x1][y1] = 0;
        } 
        else {
            // diagonal or irregular line → skip or warn
            printf("Warning: diagonal wall skipped at record %d\n", r);
        }
    }

}

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

void randomize_maze(int Maze[3][10][25]) {
    int floors[3 * 10 * 25];
    int rows[3 * 10 * 25];
    int cols[3 * 10 * 25];
    int count = 0;

    // Collect all "1" positions
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

    if (count == 0) return;

    // Shuffle indices
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

    // Calculate counts for each category
    int n25a = count * 25 / 100; // 25% → 5
    int n35  = count * 35 / 100; // 35% → 1–4
    int n25b = count * 25 / 100; // 25% → 11–12
    int n10  = count * 10 / 100; // 10% → 13–15
    int n5   = count - (n25a + n35 + n25b + n10);
    int idx = 0;

    // 25% = 5
    for (int k = 0; k < n25a; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 5;
    }

    // 35% = random {1,2,3,4}
    for (int k = 0; k < n35; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 1 + rand() % 4;
    }

    // 25% = random {11,12}
    for (int k = 0; k < n25b; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 11 + rand() % 2;
    }

    // 10% = random {13,14,15}
    for (int k = 0; k < n10; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 13 + rand() % 3;
    }

    // 5% = random {16,17}
    for (int k = 0; k < n5; k++, idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 16 + rand() % 2;
    }

    // Leftover (if rounding left any) → restore to 1
    for (; idx < count; idx++) {
        Maze[floors[idx]][rows[idx]][cols[idx]] = 1;
    }
}

void initialize_bhavana(int Maze[3][10][25]) {
    int rows_pos[10 * 25];
    int cols_pos[10 * 25];
    int count = 0;

    // Step 1: Collect all cells on first floor with value 10
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 25; c++) {
            if (Maze[0][r][c] == 10) {
                rows_pos[count] = r;
                cols_pos[count] = c;
                count++;
            }
        }
    }

    if (count == 0) return; // no cells to modify

    // Step 2: Shuffle the positions
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

    int idx = 0;

    // Step 3: Assign values in 5 steps
    int step1 = 2; // 2 → 51
    int step2 = 2; // 2 → 52
    int step3 = 2; // 2 → 53
    int step4 = 2; // 2 → 54
    int step5 = count - (step1 + step2 + step3 + step4); // remaining → 55

    for (int k = 0; k < step1; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 51;
    }
    for (int k = 0; k < step2; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 52;
    }
    for (int k = 0; k < step3; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 53;
    }
    for (int k = 0; k < step4; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 54;
    }
    for (int k = 0; k < step5; k++, idx++) {
        Maze[0][rows_pos[idx]][cols_pos[idx]] = 55;
    }
}

void cut_stairs_through_floor2(int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int numStairs) {
    for (int s = 0; s < numStairs; s++) {
        int startF = stairs[s][0];
        int startR = stairs[s][1];
        int startC = stairs[s][2];
        int endF   = stairs[s][3];
        int endR   = stairs[s][4];
        int endC   = stairs[s][5];

        // Only process stairs going from first floor to third
        if (startF == 0 && endF == 2) {
            // Floor 2 is index 1
        //    int f2 = 1;

            // Compute the exact midpoint of the stair line on floor 2
            float r_mid = (startR + endR) / 2.0f;
            float c_mid = (startC + endC) / 2.0f;

            // Find the lower and upper integer indices
            // This allows us to cover cells touched by edges or corners
            int r_floor = (int)floor(r_mid);
            int r_ceil  = (int)ceil(r_mid);
            int c_floor = (int)floor(c_mid);
            int c_ceil  = (int)ceil(c_mid);

            // Loop through all affected cells on floor 2
            for (int r = r_floor; r <= r_ceil; r++) {
                for (int c = c_floor; c <= c_ceil; c++) {
                    // Boundary check to avoid out-of-bounds
                    if (r >= 0 && r < 10 && c >= 0 && c < 25) {
                        Maze[1][r][c] = 0;  // Set intersected cell to 0
                    }
                }
            }
        }
        else {
            // Stair does not go from floor 1 to 3 → do nothing
        }
    }
}

// Roll a dice from 1 to max
int rollDice(int max) {
    return (rand() % max) + 1;
}

Player rollMovementDice(Player p) {
    // Increment round number
    p.roundNo++;

    // Roll movement dice
    int movement = rollDice(6);
    p.movePoints -= movement;

    p.movementDiceCount++;

    // Every 4th movement dice roll, roll direction dice
    if (p.movementDiceCount % 4 == 0) {
        int dirDice = rollDice(6);

        switch (dirDice) {
            case 2: p.movementDirection = 2; break; // North
            case 3: p.movementDirection = 3; break; // East
            case 4: p.movementDirection = 4; break; // South
            case 5: p.movementDirection = 5; break; // West
            case 1: case 6: /* empty → keep previous direction */ break;
        }
    }

    return p;
}

int main(){
    int walls[MAX_ROWS][MAX_COLS];
    int stairs[MAX_ROWS][MAX_COLS];
    int poles[MAX_ROWS][MAX_COLS];
    int flag[MAX_COLS];
    srand(time(NULL));
    int seed;

    Player playerA = {'A', 0, 6, 12, 2, 100,0,0}; // 2 = North
    Player playerB = {'B', 0, 9, 8, 5, 100,0,0};  // 5 = west
    Player playerC = {'C', 0, 9, 16, 3, 100,0,0}; // 3 = east

// Now you can use walls, stairs, poles, flag, and seed in your program
    int Maze[3][10][25] = {  //blocks=1 , spaces = 0
        {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,10,10,10,10},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,10,10,10,10},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,10,10,10,10}
        },
        {
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        },
        {
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0}
        }
    };

    read_text_files(walls, stairs, poles, flag, &seed);
    printMaze(Maze);
    applywalls(Maze, walls);
    randomize_maze(Maze);
    printMaze(Maze);
    initialize_bhavana(Maze);
    printMaze(Maze);
    cut_stairs_through_floor2(Maze, stairs, MAX_ROWS);
    printMaze(Maze);
    return 0;
}