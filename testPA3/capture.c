#include "chess.h"
static int isLegalCapture(int srcI, int srcJ, int trgI, int trgJ, int r) {
    int i;
    int piece = board[srcI][srcJ];//capturing piece...
    int pieceType = abs(piece);
    int pieceColor = piece > 0 ? WHITE : BLACK;
    switch (pieceType) {
    case ROOK:
        if (srcI != trgI && srcJ != trgJ)
            return 0;
        if (srcI == trgI) {//capture along a rank
            for (i = 1; i < abs(srcJ - trgJ); i++)//check middle squares
                if (srcJ<trgJ && board[srcI][srcJ + i] != EMPTY
                    || srcJ > trgJ && board[srcI][srcJ - i] != EMPTY)
                    return 0;
        }
        else//capture along a file
            for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                if (srcI<trgI && board[srcI + i][srcJ] != EMPTY
                    || srcI > trgI && board[srcI - i][srcJ] != EMPTY)
                    return 0;
        break;
    case PAWN:
        if(r)
            return (-1) * abs(srcJ - trgJ) * (srcI - trgI) == pieceColor;
        return abs(srcJ - trgJ) * (srcI - trgI) == pieceColor;
    case KNIGHT://L shape
        return abs((srcI - trgI) * (srcJ - trgJ)) == 2;
    case BISHOP:
        if (abs(srcI - trgI) != abs(srcJ - trgJ))
            return 0;//not a diagonal capture... return false...
        if (srcI < trgI) {//capturing down the board
            for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                if (srcJ < trgJ && board[srcI + i][srcJ + i] != EMPTY
                    || srcJ > trgJ && board[srcI + i][srcJ - i] != EMPTY)
                    return 0;
        }
        else//capturing up the board
            for (i = 1; i < abs(srcI - trgI);i++)//check middle squares
                if (srcJ < trgJ && board[srcI - i][srcJ + i] != EMPTY
                    || srcJ > trgJ && board[srcI - i][srcJ - i] != EMPTY)
                    return 0;
        break;
    case QUEEN:
        if (srcI == trgI || srcJ == trgJ) {//capture like a rook
            if (srcI == trgI) {//capture along a rank
                for (i = 1; i < abs(srcJ - trgJ); i++)//check middle squares
                    if (srcJ<trgJ && board[srcI][srcJ + i] != EMPTY
                        || srcJ > trgJ && board[srcI][srcJ - i] != EMPTY)
                        return 0;
            }
            else//capture along a file
                for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                    if (srcI<trgI && board[srcI + i][srcJ] != EMPTY
                        || srcI > trgI && board[srcI - i][srcJ] != EMPTY)
                        return 0;
        }
        else if (abs(srcI - trgI) == abs(srcJ - trgJ)) {//capture like a bishop
            if (srcI < trgI) {//capturing down the board
                for (i = 1; i < abs(srcI - trgI); i++)//check middle squares
                    if (srcJ < trgJ && board[srcI + i][srcJ + i] != EMPTY
                        || srcJ > trgJ && board[srcI + i][srcJ - i] != EMPTY)
                        return 0;
            }
            else//capturing up the board
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
    return 1;//legal capture
}
handleCapture(int r, int u) {
    char source[MAX_COMMAND_TOKEN_LENGTH];
    char target[MAX_COMMAND_TOKEN_LENGTH];
    char lastCharacter;
    int sourceFile, sourceRank, targetFile, targetRank, sourceI, sourceJ, targetI, targetJ;
    lastCharacter = getCommandWord(source, MAX_COMMAND_TOKEN_LENGTH);
    if (lastCharacter == '\n') {
        printf("Too few arguments for cp command! It must be in the form of cp ai bj.\n");
        return 0;
    }
    lastCharacter = getCommandWord(target, MAX_COMMAND_TOKEN_LENGTH);
    if (lastCharacter != '\n') {
        printf("Too many arguments for cp command! It must be in the form of cp ai bj.\n");
        while (getCommandWord(target, MAX_COMMAND_TOKEN_LENGTH) != '\n');
        return 0;
    }
    sourceFile = source[0];
    targetFile = target[0];
    sourceRank = source[1] - '0';
    targetRank = target[1] - '0';

    if(r){ //if the board is rotated
        sourceI = sourceRank - 1; 
        sourceJ = 7 - (sourceFile - 'a'); // -1?? bc the col assume as 9, plused with the table frame
        targetI = targetRank - 1;
        targetJ = 7 - (targetFile - 'a'); // -1??
        //turn *= -1;
    }else{
        sourceI = 8 - sourceRank;
        sourceJ = sourceFile - 'a';
        targetI = 8 - targetRank;
        targetJ = targetFile - 'a';
    }

    if (sourceI < 0 || sourceJ < 0 || targetI < 0 || targetJ < 0
        || sourceI > 7 || sourceJ > 7 || targetI > 7 || targetJ > 7) {
        printf("invalid cp arguments\n");
        return 0;
    }
    //checking the turn first
    if ((int)(board[sourceI][sourceJ] * turn) < 0) {
        printf("Turn violation, it's %s to move\n", turn == WHITE ? "WHITE" : "BLACK");
        return 0;
    }
    if ((int)board[sourceI][sourceJ] * (int)board[targetI][targetJ] > 0) { //cp color must be different
        printf("Violation, %s cannot capture its own piece.\n", turn == WHITE ? "WHITE" : "BLACK",board[sourceI][sourceJ], board[targetI][targetJ]);
        return 0;
    }
    if (board[sourceI][sourceJ] == EMPTY || board[targetI][targetJ] == EMPTY) {
        printf("Invalid capture: either source square is empty or target square is empty\n");
        return 0;
    }
    if (!isLegalCapture(sourceI, sourceJ, targetI, targetJ, r)) {
        printf("Illegal chess capture\n");
        return 0;
    }
    //end of error checking....
    board[targetI][targetJ] = board[sourceI][sourceJ];
    board[sourceI][sourceJ] = 0;

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

    turn *= -1;
    
    return 0;
}
importCapture(int r, int u, char source[], char target[]) {
    //char source[3];
    //char target[3];
    int sourceFile, sourceRank, targetFile, targetRank, sourceI, sourceJ, targetI, targetJ;
    sourceFile = source[0];
    targetFile = target[0];
    sourceRank = source[1] - '0';
    targetRank = target[1] - '0';
    if(r){ //if the board is rotated
        sourceI = sourceRank - 1; 
        sourceJ = 7 - (sourceFile - 'a'); // -1?? bc the col assume as 9, plused with the table frame
        targetI = targetRank - 1;
        targetJ = 7 - (targetFile - 'a'); // -1??
        //turn *= -1;
    }else{
        sourceI = 8 - sourceRank;
        sourceJ = sourceFile - 'a';
        targetI = 8 - targetRank;
        targetJ = targetFile - 'a';
    }
    if (sourceI < 0 || sourceJ < 0 || targetI < 0 || targetJ < 0
        || sourceI > 7 || sourceJ > 7 || targetI > 7 || targetJ > 7) {
        printf("invalid cp arguments from command-line import\n");
        return 0;
    }
    //checking the turn first
    if ((int)(board[sourceI][sourceJ] * turn) < 0) {
        printf("Turn violation from command-line import, it's %s to move\n", turn == WHITE ? "WHITE" : "BLACK");
        return 0;
    }
    if ((int)board[sourceI][sourceJ] * (int)board[targetI][targetJ] > 0) { //cp color must be different
        printf("Violation from command-line import, %s cannot capture its own piece.\n", turn == WHITE ? "WHITE" : "BLACK",board[sourceI][sourceJ], board[targetI][targetJ]);
        return 0;
    }
    if (board[sourceI][sourceJ] == EMPTY || board[targetI][targetJ] == EMPTY) {
        printf("Invalid capture from command-line import: either source square is empty or target square is empty\n");
        return 0;
    }
    if (!isLegalCapture(sourceI, sourceJ, targetI, targetJ, r)) {
        printf("Illegal chess capture from command-line import\n");
        return 0;
    }
    //end of error checking....
    board[targetI][targetJ] = board[sourceI][sourceJ];
    board[sourceI][sourceJ] = 0;

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

    turn *= -1;
    
    return 0;
}

