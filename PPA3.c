#include <stdio.h>
#include "MacUILib.h"
#include "MacUILib.c"
#include "myStringLib.h"
#include "myStringLib.c"
#include <conio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define columns 20 
#define rows 10


// GLOBAL VARIABLES
// ================================

int exitFlag; // Program Exiting Flag
char board[rows][columns];
int i, j, k, prevX, prevY, inputflag, stateMessage, moveCount;
const char goalString[] = "McMaster-ECE"; // Goal string
char* mysteryString; // Mystery string
char input; // For storing user input

struct objPos {
    int x; 
    int y; 
    char symbol; 
};
struct objPos pos;
struct objPos* itemBin;
enum FSMMode{VerticalPos, VerticalNeg, HorizontalPos, HorizontalNeg, None};
enum FSMMode motion;

// FUNCTION PROTOTYPES
// ================================

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

// list[]       The pointer to the Item Bin
// listSize     The size of the Item Bin (5 by default)
// playerPos    The pointer to the Player Object, read only.
// xRange       The maximum range for x-coordinate generation (probably the x-dimension of the game board?)
// yRange       The maximum range for y-coordinate generation (probably the y-dimension of the game board?)
// str          The pointer to the start of the Goal String (to choose the random characters from)
void GenerateItems(struct objPos list[], const int listSize, const struct objPos *playerPos, const int xRange, const int yRange, const char* str);

// MAIN PROGRAM
// ===============================
int main(void)
{

    Initialize();

    while(!exitFlag)  
    {
        GetInput();

        RunLogic();

        DrawScreen();

        LoopDelay();

    }

    CleanUp();

}

// INITIALIZATION ROUTINE
// ===============================
void Initialize(void)
{
    MacUILib_init();

    MacUILib_clearScreen();


    exitFlag = 0;  // 0 - do not exit, non-zero - exit the program
    motion = None;
    input = 0; // NULL

    pos.symbol = '@'; 
    pos.x = 10;
    pos.y = 5;
    prevX = pos.x;
    prevY = pos.y;

    itemBin = (struct objPos*)malloc(5 * sizeof(struct objPos));

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == columns - 1) {
                board[i][j] = '#'; 
            }
            else if (pos.x == j & pos.y == i) { 
                board[pos.y][pos.x] = '@'; 
            }
            else { 
                board[i][j] = ' '; 
            }
        }
    }


    srand((unsigned int)time(NULL));

    GenerateItems(itemBin, 5, &pos, columns - 1, rows - 1, goalString);
    
    mysteryString = (char*)malloc(my_strlen(goalString) + 1);
    for (i = 0; i < sizeof(goalString) - 1; i++) {
        mysteryString[i] = '?';
    }
    mysteryString[sizeof(goalString) - 1] = '\0';

    moveCount = 0;

}

// INPUT COLLECTION ROUTINE
// ===============================
void GetInput(void)
{

    // Asynchronous Input - non-blocking character read-in

    char currentInput = 0;

    if (MacUILib_hasChar() != 0)
    {
        currentInput = MacUILib_getChar();
    }

    if (currentInput != 0)
    {
        input = currentInput;
        moveCount++;
    }

}

// MAIN LOGIC ROUTINE
// ===============================
void RunLogic(void)
{
    
    if(input != 0)  // if not null character
    {
        switch(input)
        {                      
            case ' ':  // exit
                exitFlag = 1;
                break;

            case 'w':
                if (motion == VerticalNeg || motion == VerticalPos){
                    break;
                } 
                else {
                    motion = VerticalNeg;
                }
                break;
            
            case 'a':
                if (motion == HorizontalNeg || motion == HorizontalPos){
                    break;
                } 
                else {
                    motion = HorizontalNeg;
                    break;
                }
                
        
            case 's':
                if (motion == VerticalNeg || motion == VerticalPos){
                    break;
                } 
                else {
                    motion = VerticalPos;
                    break;
                }
                

            case 'd':
                if (motion == HorizontalNeg || motion == HorizontalPos){
                    break;
                }
                else {
                    motion = HorizontalPos;
                    break;
                }

        }
        input = 0;
    }
    switch(motion){

    case HorizontalNeg:
        prevX = pos.x;
        pos.x--;
        break;

    case HorizontalPos:
        prevX = pos.x;
        pos.x++;
        if (pos.x >= columns - 1) {
            pos.x = 1;      
        }
        break;

    case VerticalPos:
        prevY = pos.y;
        pos.y++;
        break;

    case VerticalNeg:
        prevY = pos.y;
        pos.y--;
        break;

    default:
        break;
    }

    if (pos.x != prevX || pos.y != prevY) {
        moveCount++; // Increment moveCount for every block moved
    }

    if (pos.x < 1){
        pos.x = columns - 2;
    } 
    else if (pos.x >= columns - 1){
        pos.x = 1;
    }

    if (pos.y >= rows - 1) {
        pos.y = 1;
    } 
    else if (pos.y < 1){
        pos.y = rows - 2;
    } 


    //      Simple Collision Algorithm
    //      1. Go through all items on board and check their (x,y) against the player object x and y.
    //      2. If a match is found, use the ASCII symbol of the collided character, and 
    //         find all the occurrences of this ASCII symbol in the Goal String
    //      3. For every occurrence, reveal its ASCII character at the corresponding location in the
    //         Collected String
    //      4. Then, determine whether the game winning condition is met.

    for (i = 0; i < 5; i++) {
    if (pos.x == itemBin[i].x && pos.y == itemBin[i].y) {
        char collidedChar = itemBin[i].symbol;
        for (j = 0; j < sizeof(goalString) - 1; j++) {
            if (goalString[j] == collidedChar && mysteryString[j] == '?') {
                mysteryString[j] = collidedChar;
                //printf("Mystery String after collision: %s\n", mysteryString);
            }
        }
        GenerateItems(itemBin, 5, &pos, columns - 1, rows - 1, goalString);
    }
}

    //      Game Winning Check Algorithm
    //      1. Check if the contents of the Collected String exactly matches that of the Goal String.
    //      2. If matched, end the game.
    //      3. Otherwise, discard the current items on the game board, and 
    //         generate a new set of random items on the board.  Game continues.
    
}
 
// DRAW ROUTINE
// ===============================

void DrawScreen(void)
{

    MacUILib_clearScreen();

    // Draw the border and player position
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            if (i == 0 || j == 0 || i == rows - 1 || j == columns - 1) {
                board[i][j] = '#';
            } else if (pos.x == j && pos.y == i) {
                board[i][j] = pos.symbol;
            } else {
                int itemFlag = 0;
                for (k = 0; k < 5; k++) {
                    if (itemBin[k].x == j && itemBin[k].y == i) {
                        board[i][j] = itemBin[k].symbol;
                        itemFlag = 1;
                    }
                }
                if (!itemFlag) {
                    board[i][j] = ' ';
                }
            }
        }
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            MacUILib_printf("%c", board[i][j]);
        }
        MacUILib_printf("\n");
    }

    MacUILib_printf("Move Count: %d\n", moveCount);
    MacUILib_printf("Mystery String: %s\n", mysteryString);

    if (my_strcmpOrder(mysteryString,goalString)){
        MacUILib_clearScreen();
        MacUILib_printf("** CONGRATULATIONS YOU WON **");
    }

}

// DELAY ROUTINE
// ===============================
void LoopDelay(void)
{
    int delay = 100000; 

    if (motion == VerticalPos || motion == VerticalNeg) {
        delay = 150000; 
    }

    MacUILib_Delay(delay);
}

// TEAR-DOWN ROUTINE
// ===============================
void CleanUp(void)
{

    free(mysteryString);
    free(itemBin);

    MacUILib_uninit();
}

// The Item Generation Routine
////////////////////////////////////
void GenerateItems(struct objPos list[], const int listSize, const struct objPos *playerPos, const int xRange, const int yRange, const char* str)
{

    // Random Non-Repeating Item Generation Algorithm
    ////////////////////////////////////////////////////


    int i = 0;

    while (i < listSize)
    {
        int candidateX, candidateY;
        char randomSymbol;
        int charChosen = 0;

        if (i < 2) {
            int randomIndex = rand() % my_strlen(str);
            randomSymbol = str[randomIndex];
        } else {
            randomSymbol = rand() % (126 - 33 + 1) + 33;

            if (randomSymbol == playerPos->symbol || randomSymbol == '#' || randomSymbol == ' ') {
                continue;
            }
        }

        int j;
        for (j = 0; j < i; j++)
        {
            if (list[j].symbol == randomSymbol)
            {
                charChosen = 1;
                break;
            }
        }

        if (!charChosen)
        {
            candidateX = rand() % xRange;
            candidateY = rand() % yRange;

            if (candidateX != playerPos->x || candidateY != playerPos->y)
            {
                list[i].x = candidateX;
                list[i].y = candidateY;
                list[i].symbol = randomSymbol;
                i++;
            }
        }
    }
}
