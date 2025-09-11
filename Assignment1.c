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

void read_text_files() {
    FILE *f;
    char line[MAX_LINE];
    int i, j;

    //Walls
    int walls[MAX_ROWS][MAX_COLS];
    int walls_sizes[MAX_ROWS];
    int walls_rows = 0;

    f = fopen("walls.txt", "r");
    if (!f) { perror("walls.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue; // skip comment lines
        walls_sizes[walls_rows] = parse_line(line, walls[walls_rows], MAX_COLS);
        walls_rows++;
        if (walls_rows >= MAX_ROWS) break;
    }
    fclose(f);

    //Stairs
    int stairs[MAX_ROWS][MAX_COLS];
    int stairs_sizes[MAX_ROWS];
    int stairs_rows = 0;

    f = fopen("stairs.txt", "r");
    if (!f) { perror("stairs.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue; // skip comment lines
        stairs_sizes[stairs_rows] = parse_line(line, stairs[stairs_rows], MAX_COLS);
        stairs_rows++;
        if (stairs_rows >= MAX_ROWS) break;
    }
    fclose(f);

    //Poles
    int poles[MAX_ROWS][MAX_COLS];
    int poles_sizes[MAX_ROWS];
    int poles_rows = 0;

    f = fopen("poles.txt", "r");
    if (!f) { perror("poles.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue; // skip comment lines
        poles_sizes[poles_rows] = parse_line(line, poles[poles_rows], MAX_COLS);
        poles_rows++;
        if (poles_rows >= MAX_ROWS) break;
    }
    fclose(f);

    //Flag
    int flag[MAX_COLS];
    int flag_size = 0;

    f = fopen("flag.txt", "r");
    if (!f) { perror("flag.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue; // skip comment lines
        flag_size = parse_line(line, flag, MAX_COLS);
        break;
    }
    fclose(f);

    //Seed
    int seed=0;
    int seed_size = 0;

    f = fopen("seed.txt", "r");
    if (!f) { perror("seed.txt"); exit(1); }
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue; // skip comment lines
        int temp[MAX_COLS];        // temporary array to parse
        int count = parse_line(line, temp, MAX_COLS);
        seed = temp[0];
        break;
    }
    fclose(f);
}

int main(){
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
    return 0;
}