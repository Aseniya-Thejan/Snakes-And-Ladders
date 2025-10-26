#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_LINE 100     // maximum characters in one line
#define MAX_ROWS 20      // maximum number of rows per file     //For file handling parts
#define MAX_COLS 20      // maximum numbers per row
#define STEPSIZE 5       
#define MPCAP 250

typedef struct {
        char name;              // Player name
        int floor;              // Current floor
        int widthBlock;         // Current row
        int heightBlock;        // Current column
        int movementDirection;  // 2=north, 3=east, 4=south, 5=west
        int movementPoints;     // Remaining movement points
        int movementDiceCount;  // Keep track of to trow direction dise         
        int hasEntered;         // 0 = Player is in starting area  1 = playing
        int inBHAWANA;          // Is player is in Bhawana
        int isTriggered;        // Is player is in Trigger mode
        int isDisoriented;      // Is player is in Disoriented mode
        int disorientedRounds;  // Remaining disorieted rounds
        int isFoodPoisoned;     // Is player is in FoodPoisoned mode
        int foodPoisonRounds;   // Remaining foodpoisoned rounds
    } Player;

//Function Prototypes 
void validateFlag(int flag[3], int Maze[3][10][25]);
int parse_line(char line[], int arr[], int max);
void read_text_files(int walls[MAX_ROWS][MAX_COLS], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS], int flag[MAX_COLS], int *seed );
void applywalls(int Maze[3][10][25], int walls[MAX_ROWS][MAX_COLS]);
void printMaze(int maze[3][10][25]);
void randomize_maze(int Maze[3][10][25]);
void initialize_bhavana(int Maze[3][10][25]);   
void cut_stairs_through_floor2(int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int numStairs);
Player movePlayer(Player p, int movement, int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS],int numpoles,int numrows,int flag[MAX_COLS]);
int rollDice(int max);
const char* getDirection(int num);
Player rollMovementDice(Player p, int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int numrows, int poles[MAX_ROWS][MAX_COLS], int numpoles,int flag[MAX_COLS]);
Player checkpoles(Player p, int poles[MAX_ROWS][MAX_COLS], int numPoles, int Maze[3][10][25], int *poleFailed);
Player checkStairs(Player p, int stairs[MAX_ROWS][MAX_COLS], int numStairs, int Maze[3][10][25],int *stairFailed, int flag[3]);
int loadStairs(int stairs[MAX_ROWS][MAX_COLS]);
int loadpoles(int poles[MAX_ROWS][MAX_COLS]);
void updatestairdirection(int stairs[MAX_ROWS][MAX_COLS], int numRows, int rdn, Player p,int temp,int Maze[3][10][25],int poles[MAX_ROWS][MAX_COLS],int numPoles,int flag[MAX_COLS]);
int randInRange(int min, int max);
const char* getcelltype(int value);
Player movePlayer(Player p, int movement, int Maze[3][10][25], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS], int numpoles, int numrows, int flag[MAX_COLS]);
void checkCapture(int numPlayers, Player players[], int currentIndex, int entryFloor[], int entryRow[], int entryCol[], int Maze[3][10][25]);
const char* checkDirection(int temp);
const char* getCellDescription(int cellValue);
void Style();
void Playgame(int walls[MAX_ROWS][MAX_COLS], int stairs[MAX_ROWS][MAX_COLS], int poles[MAX_ROWS][MAX_COLS], int Maze[3][10][25], Player players[3], int flag[MAX_COLS], int entryFloor[], int entryRow[], int entryCol[]) ;

#endif