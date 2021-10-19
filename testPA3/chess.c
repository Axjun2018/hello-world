#include "chess.h"
square board[8][8];
int turn;
static void resetBoard(int r) {// pass r, cover the situation of board rotation
    turn = WHITE;//WHITE=1 moves first
    int i, j;
    for (i = 2; i < 6;i++)
        for (j = 0;j < 8;j++)
            board[i][j] = EMPTY;
    if(r){      //if board is rotated
        for (j = 0;j < 8;j++) {
            board[1][j] = WHITE * PAWN;
            board[6][j] = BLACK * PAWN;
        }
        board[0][0] = board[0][7] = WHITE * ROOK;
        board[7][0] = board[7][7] = BLACK * ROOK;
        board[0][1] = board[0][6] = WHITE * KNIGHT;
        board[7][1] = board[7][6] = BLACK * KNIGHT;
        board[0][2] = board[0][5] = WHITE * BISHOP;
        board[7][2] = board[7][5] = BLACK * BISHOP;
        board[0][4] = WHITE * QUEEN; //some different with original board
        board[7][3] = BLACK * QUEEN;
        board[0][3] = WHITE * KING; //some different with original board
        board[7][4] = BLACK * KING;
    }else{      //if board is not rotated
        for (j = 0;j < 8;j++) {
            board[1][j] = BLACK * PAWN;
            board[6][j] = WHITE * PAWN;
        }
        board[0][0] = board[0][7] = BLACK * ROOK;
        board[7][0] = board[7][7] = WHITE * ROOK;
        board[0][1] = board[0][6] = BLACK * KNIGHT;
        board[7][1] = board[7][6] = WHITE * KNIGHT;
        board[0][2] = board[0][5] = BLACK * BISHOP;
        board[7][2] = board[7][5] = WHITE * BISHOP;
        board[0][3] = BLACK * QUEEN;
        board[7][3] = WHITE * QUEEN;
        board[0][4] = BLACK * KING;
        board[7][4] = WHITE * KING;
    }
}
char getCommandWord(char command[], int maxLength) {
    char lastCharacter;//either space or new line
    int i;
    for (i = 0; (lastCharacter = getchar()) == ' '; i++);//skip leading white spaces
    if (lastCharacter == '\n') {
        command[0] = '\0';
        return lastCharacter;
    }
    command[0] = lastCharacter;
    for (i = 1; i < maxLength - 1 && (command[i] = getchar()) != ' ' && command[i] != '\n'; i++);
    lastCharacter = command[i];
    command[i] = '\0';
    return lastCharacter;
}

main(int argc, char** argv) {
    int w = 0, b = 0, r = 0, u = 0, t = 0;
    int index;
    char *import;//get a string of moves and captures
    
    for(index = 1; index < argc; index++){
        if(strstr(argv[index], "-w")){ //give name of whitePiecePlayer
            w = 1;
            strcpy(firstPlayerName,argv[index+1]); //printf("%s\n", firstPlayerName);  //===test===
        }else if(strstr(argv[index], "-b")){ //give name of blackPiecePlayer
            b = 1;
            strcpy(secondPlayerName, argv[index+1]); //printf("%s\n", secondPlayerName); //===test===
        }else if(strstr(argv[index], "-r")){// rotate chess board for 180 degrees
            r = 1;
        }else if(strstr(argv[index], "-i")){// import moves/captures, and show result on chess board
            //printf("%s\n", argv[index+1]); //===test===
            import = argv[index+1]; //copy the string after -i flag
            //printf("%s\n", import);//===test===
        }else if(strstr(argv[index], "-u")){//allows under-promotion of pawns to queen, knight, bishop and rook
            u = 1;
        }else if(strstr(argv[index], "-t")){ // timer for each player
            t = 1;
            //state = NOT_STARTED;
            whiteTimeLimit = atoi(argv[index+1])*60; //convert string to int
            blackTimeLimit = atoi(argv[index+1])*60;
            printf("\nTime left for white player: %.2f seconds\n", whiteTimeLimit);
            printf("Time left for black player: %.2f seconds\n", blackTimeLimit);
            //printf("limited time is: %.2f\n", whiteTimeLimit);  //===test===
        }
    }
    
    /*
    * You can access all the command-line arguments here
    * argv[1], argv[2], ...
    * Example of import option: -i d2d4,d7d5,e2e4,d5xe4,Ng1h3,Ke8d7,Qd1d3 //N/K can be ignored
    * which is equivalent to a new game started this way:
    * mv d2 d4
    * mv d7 d5
    * mv e2 e4
    * cp d5 e4
    * mv g1 h3
    * mv e8 d7
    * mv d1 d3
    */
    char command[MAX_COMMAND_TOKEN_LENGTH];
    char lastCharacter;
    resetBoard(r);
    printf("Start the game using a mv command! Valid commands are quit, show, mv and cp.\n");

    while (1) {//infinite while loop...
        printf(">>");
        lastCharacter = getCommandWord(command, MAX_COMMAND_TOKEN_LENGTH);
        if (strcmp(command, "quit") == 0)//if command === "quit"
            break;
        else if (!strcmp(command, "mv"))//if command == "mv"
        {
            if (lastCharacter == '\n')//mv\n
                printf("Too few arguments for mv command! It must be in the form of mv ai bj.\n");
            else
                handleMove(r, u);
        }
        else if (!strcmp(command, "cp"))//if command == "cp"
        {
            if (lastCharacter == '\n')//cp\n
                printf("Too few arguments for cp command! It must be in the form of cp ai bj.\n");
            else
                handleCapture(r, u);
        }
        else if (!strcmp(command, "show"))//if command == "show"
        {
            if (lastCharacter != '\n') {//show x\n
                printf("Too many arguments for show command! It must be in the form of show.\n");
                while (getCommandWord(command, MAX_COMMAND_TOKEN_LENGTH) != '\n');//skip the ramainder of the invalid show command...
            }
            else{
                handleShow(w, b, r, import, u, t);
                //empty import content
                import = NULL; //import can be only used once: handleShow only print import result at 1st time printing
            }
        }
        else if(!(strstr(command, "N") || strstr(command, "B") || strstr(command, "R") || strstr(command, "Q"))){
            //legal input for pawn's promotion
        }
        else {
            printf("invalid command! Your command must start either with quit, show, mv or cp.\n");
            while (lastCharacter != '\n')//skip the remainder of my invalid command...
                lastCharacter = getCommandWord(command, MAX_COMMAND_TOKEN_LENGTH);
        }
    }
}


