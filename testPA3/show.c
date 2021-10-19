#include"chess.h"
handleShow(int w, int b, int r, char *import, int u, int t) {
    int i = 0, j, commaCount=0, numOfWords, l, tokenLen;
    char *token, **importCopy;
    char substring1[3], substring2[3];
    
    // get imported game, and show on chess board
    if(import != NULL){
        //get number of words of import content
        for(i = 0; i < strlen(import); i++){ //printf("%c", import[i]);      //===test
            if(import[i] == ','){ //count # of comma, then +1: printf("find comma!\n");      //===test===
                commaCount++;
            }
        }
        numOfWords = commaCount + 1; //printf("\nthe number of words/pointers of import is: %d\n", numOfWords);//===test===
  
        importCopy = (char**)malloc(numOfWords * sizeof(char*));
        token = strtok(import, ","); //token each word that was splited by comma
        while (token != NULL){   
            //printf("In show(): %s\n", token); //===test===
            importCopy[i] = (char*)malloc(strlen(token) * sizeof(char)); //copy each token from import to importCopy
            strcpy(importCopy[i], token);
            //check if the import is valid
            tokenLen = strlen(importCopy[i]);
            if(tokenLen == 4){ //pawn move
                memcpy(substring1, &*importCopy[i], 2);
                substring1[2] = '\0';       //printf("substring1 = %s\t", substring1);
                memcpy(substring2, &*importCopy[i]+2, 2);
                substring2[2] = '\0';       //printf("substring2 = %s\t\n", substring2);
                importMove(r, u, substring1, substring2);
            }else if(tokenLen == 5){
                if( strstr(importCopy[i], "x") || strstr(importCopy[i], "X")){//pawn cp
                    memcpy(substring1, &*importCopy[i], 2);
                    substring1[2] = '\0';       //printf("substring1 = %s\t", substring1);
                    memcpy(substring2, &*importCopy[i]+3, 2);
                    substring2[2] = '\0';       //printf("substring2 = %s\t\n", substring2);
                    importCapture(r, u, substring1, substring2);
                }else{//non-pawn move, ignore first letter
                    memcpy(substring1, &*importCopy[i]+1, 2);
                    substring1[2] = '\0';       //printf("substring1 = %s\t", substring1);
                    memcpy(substring2, &*importCopy[i]+3, 2);
                    substring2[2] = '\0';       //printf("substring2 = %s\t\n", substring2);
                    importMove(r, u, substring1, substring2);
                }
            }else if(tokenLen == 6 && (strstr(importCopy[i], "x") || strstr(importCopy[i], "X"))){//non-pawn x pawn cp
                if(isdigit(importCopy[i]+1)){//if the 2nd char is a number: pawn cp non-pawn
                    memcpy(substring1, &*importCopy[i], 2);
                    substring1[2] = '\0';       //printf("substring1 = %s\t", substring1);
                    memcpy(substring2, &*importCopy[i]+3, 2);
                    substring2[2] = '\0';       //printf("substring2 = %s\t\n", substring2);
                    importCapture(r, u, substring1, substring2);
                }else if(isalpha(importCopy[i]+1)){//if the 2nd char is a letter: non-pawn cp pawn
                    memcpy(substring1, &*importCopy[i]+1, 2);
                    substring1[2] = '\0';       //printf("substring1 = %s\t", substring1);
                    memcpy(substring2, &*importCopy[i]+4, 2);
                    substring2[2] = '\0';       //printf("substring2 = %s\t\n", substring2);
                    importCapture(r, u, substring1, substring2);
                }
            }else if(tokenLen == 7 && (strstr(importCopy[i], "x") || strstr(importCopy[i], "X"))){//non-pawn cp non-pawn
                char substring1[3];
                char substring2[3];
                memcpy(substring1, &*importCopy[i]+1, 2);
                substring1[2] = '\0';
                memcpy(substring2, &*importCopy[i]+4, 2);
                substring2[2] = '\0';
                importCapture(r, u, substring1, substring2);
            }
            token = strtok(NULL, ",");
            i++;
        }
    }

    if (r){ // rotate the board
        if(w) //white at top
            printf("%s\n", firstPlayerName);
        printf("board\t");
        for (i = 0; i < 8; i++)//file labels...
            printf("%c%c", 'h' - i, i == 7 ? '\n' : '\t');
        for (i = 0; i < 8; i++)//table itself
            for (j = 0; j < 9; j++)
                printf("%d%c", j == 0 ? i+1 : board[i][j - 1], j == 8 ? '\n' : '\t');
        if(b) //black at bottom
            printf("%s\n", secondPlayerName);
    }else{
        if(b) //black at top
            printf("%s\n", secondPlayerName);
        printf("board\t");
        for (i = 0; i < 8; i++)//file labels...
            printf("%c%c", 'a' + i, i == 7 ? '\n' : '\t');
        for (i = 0; i < 8; i++)//table itself
            for (j = 0; j < 9; j++)
                printf("%d%c", j == 0 ? 8 - i : board[i][j - 1], j == 8 ? '\n' : '\t');
        if(w)  //white at bottom
            printf("%s\n", firstPlayerName);
        }

    // set timer for each player    
    if(t){
        if(state == STARTED){
            if(turn == WHITE){
                //printf("%.2f\n", current); //===test===
                blackTimeLimit -= (time(NULL)-current); // timeLimit - (current - oldCurrent)
                printf("Time left for white player: %.2f seconds\n", whiteTimeLimit);
                printf("Time left for black player: %.2f seconds\n", blackTimeLimit);
            }else{
                //printf("%d\n", current); //===test===
                whiteTimeLimit -= (time(NULL)-current);
                current = time(NULL);
                printf("Time left for white player: %.2f seconds\n", whiteTimeLimit);
                printf("Time left for black player: %.2f seconds\n", blackTimeLimit);
            }
        }else{
            current = time(NULL);
            state = STARTED;
            //printf("%.2f\n", current); //===test===
            //printf("Time left for white player: %.2f seconds\n", whiteTimeLimit); //===test===
            //printf("Time left for black player: %.2f seconds\n", blackTimeLimit); //===test===
        }
    }
}
