#include "chess.h"
static int isLegalMove(int srcI, int srcJ, int trgI, int trgJ, int r) {
    int i;
    int piece = board[srcI][srcJ];//moved piece...
    int pieceType = abs(piece); //the absolute piece value indicates the piece type
    int pieceColor = piece > 0 ? WHITE : BLACK; //1:-1
    switch (pieceType) {
    case ROOK:
        if (srcI != trgI && srcJ != trgJ) //if no move happen, return (invalid)
            return 0;
        if (srcI == trgI) {//move along a rank
            for (i = 1; i < abs(srcJ - trgJ); i++)//check middle squares
                if (srcJ<trgJ && board[srcI][srcJ + i] != EMPTY
                    || srcJ > trgJ && board[srcI][srcJ - i] != EMPTY)
                    return 0;
        }
        else//move along a file
            for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                if (srcI<trgI && board[srcI + i][srcJ] != EMPTY
                    || srcI > trgI && board[srcI - i][srcJ] != EMPTY)
                    return 0;
        break;
    case PAWN:
        if (srcJ != trgJ)
            return 0;
        if(r){ //if board is rotated
            if (pieceColor == WHITE) {  //white is on top
                if (srcI == 1) //starting position
                    return trgI == 2 || trgI == 3;
                else
                    return trgI == srcI + 1;
            }
            else {      //black is at bottom
                if (srcI == 6)//starting position
                    return trgI == 5 || trgI == 4;
                else
                    return trgI == srcI - 1;
            }
        }else{
            if (pieceColor == BLACK) {
                if (srcI == 1)//starting position
                    return trgI == 2 || trgI == 3;
                else
                    return trgI == srcI + 1;
            }
            else {
                if (srcI == 6)//starting position
                    return trgI == 5 || trgI == 4;
                else
                    return trgI == srcI - 1;
            }
        }
        break;
    case KNIGHT://L shape: srcI-trgI = 2,-2,1,-1; srcJ-trgJ = 1, -1, 2, -2
        return abs((srcI - trgI) * (srcJ - trgJ)) == 2;
    case BISHOP:
        if (abs(srcI - trgI) != abs(srcJ - trgJ))
            return 0;//not a diagonal move... return false...
        if (srcI < trgI) {//moving down the board
            for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                if (srcJ < trgJ && board[srcI + i][srcJ + i] != EMPTY
                    || srcJ > trgJ && board[srcI + i][srcJ - i] != EMPTY)
                    return 0;
        }
        else//moving up the board
            for (i = 1; i < abs(srcI - trgI);i++)//check middle squares
                if (srcJ < trgJ && board[srcI - i][srcJ + i] != EMPTY
                    || srcJ > trgJ && board[srcI - i][srcJ - i] != EMPTY)
                    return 0;
        break;
    case QUEEN:
        if (srcI == trgI || srcJ == trgJ) {//move like a rook
            if (srcI == trgI) {//move along a rank
                for (i = 1; i < abs(srcJ - trgJ); i++)//check middle squares
                    if (srcJ<trgJ && board[srcI][srcJ + i] != EMPTY
                        || srcJ > trgJ && board[srcI][srcJ - i] != EMPTY)
                        return 0;
            }
            else//move along a file
                for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                    if (srcI<trgI && board[srcI + i][srcJ] != EMPTY
                        || srcI > trgI && board[srcI - i][srcJ] != EMPTY)
                        return 0;
        }
        else if (abs(srcI - trgI) == abs(srcJ - trgJ)) {//move like a bishop
            if (srcI < trgI) {//moving down the board
                for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                    if (srcJ < trgJ && board[srcI + i][srcJ + i] != EMPTY
                        || srcJ > trgJ && board[srcI + i][srcJ - i] != EMPTY)
                        return 0;
            }
            else//moving up the board
                for (i = 1; i < abs(srcI - trgI);i++)//check middle squares
                    if (srcJ < trgJ && board[srcI - i][srcJ + i] != EMPTY
                        || srcJ > trgJ && board[srcI - i][srcJ - i] != EMPTY)
                        return 0;
        }
        else
            return 0;
        break;
    case KING:
        return abs(srcI - trgI) + abs(srcJ - trgJ) == 1
            || abs(srcI - trgI) * abs(srcJ - trgJ) == 1;
    }
    return 1;//legal move
}

handleMove(int r, int u) {
    char source[MAX_COMMAND_TOKEN_LENGTH];//placeholder for argument 1 (initial square)
    char target[MAX_COMMAND_TOKEN_LENGTH];//placeholder for argument 2 (final square)
    char lastCharacter;
    int file;
    int sourceFile, sourceRank, targetFile, targetRank, sourceI, sourceJ, targetI, targetJ;
    //source SQUARE: board[sourceI][sourceJ]
    //target SQUARE: board[targetI][targetJ]
    lastCharacter = getCommandWord(source, MAX_COMMAND_TOKEN_LENGTH); //get src command
    if (lastCharacter == '\n') {
        printf("Too few arguments for mv command! It must be in the form of mv ai bj.\n");
        return 0;
    }
    lastCharacter = getCommandWord(target, MAX_COMMAND_TOKEN_LENGTH); //get target command
    if (lastCharacter != '\n') {
        printf("Too many arguments for mv command! It must be in the form of mv ai bj.\n");
        while (getCommandWord(target, MAX_COMMAND_TOKEN_LENGTH) != '\n');//skip the rest of illegal command..
        return 0;
    }
    sourceFile = source[0];//source = "a5", sourceFile = 'a'
    targetFile = target[0];
    sourceRank = source[1] - '0';//source = "a5", sourceRank = 5
    targetRank = target[1] - '0';
    //board[sourceI][sourceJ]: source square...
    //board[targetI][targetJ]: target square...
    if(r){ //if the board is rotated
        sourceI = sourceRank - 1; 
        sourceJ = 7 - (sourceFile - 'a'); // -1: bc the col assume as 9, plused with the table frame
        targetI = targetRank - 1;
        targetJ = 7 - (targetFile - 'a'); // -1
    }else{
        sourceI = 8 - sourceRank;
        sourceJ = sourceFile - 'a'; // +1?? bc the col assume as 9, plused with the table frame
        targetI = 8 - targetRank;
        targetJ = targetFile - 'a'; // +1??
    }
    
    //move out of board
    if (sourceI < 0 || sourceJ < 0 || targetI < 0 || targetJ < 0
        || sourceI > 7 || sourceJ > 7 || targetI > 7 || targetJ > 7) {
        printf("Invalid mv arguments\n");
        return 0;
    }
    //checking the turn first
    if ((int)(board[sourceI][sourceJ] * turn) < 0) { // enum * int, enum maybe unsigned which is non-negative, so cast square to int
        printf("Turn violation, it's %s to move\n", turn == WHITE ? "white" : "black");
        return 0;
    }
    if (board[sourceI][sourceJ] == EMPTY || board[targetI][targetJ] != EMPTY) {
        printf("Invalid move: either source square is empty or target square is not empty\n");
        return 0;
    }
    if (!isLegalMove(sourceI, sourceJ, targetI, targetJ, r)) {
        printf("Illegal chess move\n");
        return 0;
    }
    //end of error checking....

    board[targetI][targetJ] = board[sourceI][sourceJ]; // target square occepied by source piece
    board[sourceI][sourceJ] = EMPTY;
    
    if(u){ //if pawn's promotion is allowed
        if(r){ //if board is rotated
            if(targetI == 7 && board[targetI][targetJ] == WHITE * PAWN) //white pawn at rank8 [0th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
            if(targetI == 0 && board[targetI][targetJ] == BLACK * PAWN) //black pawn at rank1 [7th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
        }else{ // the board is not rotated
            if(targetI == 0 && board[targetI][targetJ] == WHITE * PAWN) //white pawn at rank8 [7th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
            if(targetI == 7 && board[targetI][targetJ] == BLACK * PAWN) //black pawn at rank1 [0th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
            }
        }

    turn *= -1;//WHITE --> BLACK and BLACK --> WHITE
    return 0;
}

int promote(int piece){
    //printf("piece is %d\n", piece); //===test===
    printf("Enter one of the piece names: N, B, R, Q to promote your pawn (N: knight, B: bishop, R: rook, Q: queen):\n");
    char promotion[2];
    int promoted, copyPiece = piece, startPromotion = 1; //copyPiece prevent the piece from unknown modifying
    promotion[1] = '\0';
    while (startPromotion){
        scanf("%c", &promotion[0]);
        if(strstr(promotion,"N")){
            promoted = copyPiece * KNIGHT;
            //printf("promoted piece = %d\n", promoted); //===test===
            startPromotion = 0;
        }else if(strstr(promotion,"B")){
            promoted = copyPiece * BISHOP;
            startPromotion = 0;
        }else if(strstr(promotion,"R")){
            promoted = copyPiece * ROOK;
            startPromotion = 0;
        }else if (strstr(promotion,"Q")){
            promoted = copyPiece * QUEEN;
            startPromotion = 0;
        }else{
            printf("Invalid promotion!\n Please re-enter one capital letter (N, B, R, Q) for promotion:\n");
        }
    }
    return promoted;
}

importMove(int r, int u, char source[], char target[]) {
    //char source[3];//placeholder for argument 1 (initial square)
    //char target[3];//placeholder for argument 2 (final square)
    int sourceFile, sourceRank, targetFile, targetRank, sourceI, sourceJ, targetI, targetJ;
    //source SQUARE: board[sourceI][sourceJ]
    //target SQUARE: board[targetI][targetJ]

    sourceFile = source[0];//source = "a5", sourceFile = 'a'
    targetFile = target[0];
    sourceRank = source[1] - '0';//source = "a5", sourceRank = 5
    targetRank = target[1] - '0';
    //board[sourceI][sourceJ]: source square...
    //board[targetI][targetJ]: target square...
    if(r){ //if the board is rotated
        sourceI = sourceRank - 1; 
        sourceJ = 7 - (sourceFile - 'a'); // -1?? bc the col assume as 9, plused with the table frame
        targetI = targetRank - 1;
        targetJ = 7 - (targetFile - 'a'); // -1??
    }else{
        sourceI = 8 - sourceRank;
        sourceJ = sourceFile - 'a'; // +1?? bc the col assume as 9, plused with the table frame
        targetI = 8 - targetRank;
        targetJ = targetFile - 'a'; // +1??
    }
    
    //move out of board
    if (sourceI < 0 || sourceJ < 0 || targetI < 0 || targetJ < 0
        || sourceI > 7 || sourceJ > 7 || targetI > 7 || targetJ > 7) {
        printf("Invalid mv arguments from command-line import\n");
        return 0;
    }
    //checking the turn first
    if ((int)(board[sourceI][sourceJ] * turn) < 0) { // enum * int, enum maybe unsigned which is non-negative, so cast square to int
        printf("Turn violation from command-line import, it's %s to move\n", turn == WHITE ? "white" : "black");
        return 0;
    }
    if (board[sourceI][sourceJ] == EMPTY || board[targetI][targetJ] != EMPTY) {
        printf("Invalid move from command-line import: either source square is empty or target square is not empty\n");
        return 0;
    }
    if (!isLegalMove(sourceI, sourceJ, targetI, targetJ, r)) {
        printf("Illegal chess move from command-line import\n");
        return 0;
    }
    //end of error checking....

    board[targetI][targetJ] = board[sourceI][sourceJ]; // target square occepied by source piece
    board[sourceI][sourceJ] = EMPTY;

    if(u){ //if pawn's promotion is allowed
        if(r){ //if board is rotated
            if(targetI == 7 && board[targetI][targetJ] == WHITE * PAWN) //white pawn at rank8 [0th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
            if(targetI == 0 && board[targetI][targetJ] == BLACK * PAWN) //black pawn at rank1 [7th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
        }else{ // the board is not rotated
            if(targetI == 0 && board[targetI][targetJ] == WHITE * PAWN) //white pawn at rank8 [7th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
            if(targetI == 7 && board[targetI][targetJ] == BLACK * PAWN) //black pawn at rank1 [0th row] can be promoted
                board[targetI][targetJ] = promote(board[targetI][targetJ]);
            }
        }

    turn *= -1;//WHITE --> BLACK and BLACK --> WHITE
    return 0;
}


