#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
//#include<unistd.h>
#include<time.h>
#include <ctype.h> //check isdigit or isalpha
#define MAX_COMMAND_TOKEN_LENGTH 6
#define WHITE 1
#define BLACK -1
#define NOT_STARTED 0 //for timer
#define STARTED 1 //for timer
#define SECONDS_PER_MIN 60.0
double whiteTimeLimit, blackTimeLimit, current;
int state;
char firstPlayerName[100], secondPlayerName[100];
typedef enum {
    EMPTY,     //0
    PAWN,      //1
    KNIGHT,    //2
    BISHOP,    //3
    ROOK,      //4
    QUEEN,     //5
    KING       //6
}square;

typedef enum {
    NONE = 0,
    WHITE_SHORT = 1,
    WHITE_LONG = 2,
    BLACK_SHORT = 4,
    BLACK_LONG = 8
}castle; //-c
extern square board[8][8];
extern int turn;
char getCommandWord(char command[], int maxLength);
handleShow(int w, int b, int r, char *import, int u, int t);//modified
handleCapture(int r, int u);
handleMove(int r, int u);
importMove(int r, int u, char source[], char target[]); //add-on
importCapture(int r, int u, char source[], char target[]); //add-on
int promote(int piece);
