#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 100     // maximum characters in one line
#define MAX_ROWS 20      // maximum number of rows per file
#define MAX_COLS 20      // maximum numbers per row

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
        parse_line(line, stairs[rows], MAX_COLS);
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

int main(){
    int walls[MAX_ROWS][MAX_COLS];
    int stairs[MAX_ROWS][MAX_COLS];
    int poles[MAX_ROWS][MAX_COLS];
    int flag[MAX_COLS];
    int seed;
    // Now you can use walls, stairs, poles, flag, and seed in your program
    int Maze[3][10][25] = {  //blocks=1 , spaces = 0
        {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1}
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
    printMaze(Maze);
    return 0;
}