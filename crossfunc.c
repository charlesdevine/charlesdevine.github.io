// Charlie Devine, 11/1/23, Fund Comp Lab 8, Crossword Functions
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "crossfunc.h"

// read in words from user
int readwords(char words[][max+1]){
    printf("Enter a list of words:\n");
    int count=0;
    char newword[max];
    // while word count hasn't reached 20
    while(1){
        printf("Enter a word: ");
        scanf("%s", &newword);
        // stop if '.' is typed
        if((newword[0])== '.')
            break;
        // check validity of word
        bool B= true;
        // flag to mark if work contains non-letters or too long/short
        int flag=1;
        while(B){
            flag=1;
            // check if word is too long
            if(strlen(newword)>=16){
                printf("Error, word too long\n");
                B= false;
                flag=0;
            }
            // check if word is too short
            else if(strlen(newword)<2){
                printf("Error, word too short\n");
                B= false;
                flag=0;
            }
            // check if word contains non-letters
            else{

                for(int i=0; i<strlen(newword);i++){
                 if(!isalpha(newword[i])){
                    printf("Error, cannot except non-letters\n");
                    B= false;
                    flag=0;
                    }
                }
            }
            // if word is good size and has all letters a-z (checked by flag)
            if(flag==1){
                strcpy(words[count], newword);
                count++;
                B= false;
            }
        }
        // while word count hasn't reached 20
        if(count>=20){
            break;
        }
    }
    // once words 2D array made, change to all upper by indexing through each letter ERROR BEFORE HERE
    for(int i=0; i<count; i++){
        for(int j=0; j<sizeof(words[i]); j++){
            words[i][j]= toupper(words[i][j]);
        }
    }
    return count;
}

// organize words by descending length and store in struct, then print
void organizewords(char words[][max+1], Cross arr[], int count){
    int k=0;
    // go by descending length order from max down, then go through words array
    for(int i=15; i>0; i--){
        for(int j=0; j<count; j++){
            if(strlen(words[j])==i){
                // store both word and length in struct
                strcpy(arr[k].word, words[j]);
                arr[k].length = i;
                k++;
            }
        }   
    }
    strcpy(arr[0].direction, "horizontal");
    strcpy(arr[1].direction, "vertical");
    // print words in descending length order
    printf("\n");
    for(int i=0; i<count; i++){
        printf("%s\n", arr[i].word);
    }
}

// create blank solution board
void makesolboard(char solboard[max+2][max+2]){
    for(int i=0; i<=max+1; i++){
        for(int j=0; j<=max+1; j++){
            if(i==0 || i==max+1)
                solboard[i][j]='_';
            else if(j==0 || j==max+1)
                solboard[i][j]='|';
            else
                solboard[i][j]='.';
        }
    }
}

// display solution board
void showsolboard(char solboard[max+2][max+2]){
    printf("Solution:\n");
    for(int i= 0;  i<=max+1; i++){
        for(int j= 0; j<=max+1; j++){
            printf("%c", solboard[i][j]);
        }
        printf("\n");
    }
}

// create blank puzzle board
void makepuzzboard(char puzzboard[max+2][max+2]){
    for(int i=0; i<=max+1; i++){
        for(int j=0; j<=max+1; j++){
            if(i==0 || i==max+1)
                puzzboard[i][j]='_';
            else if(j==0 || j==max+1)
                puzzboard[i][j]='|';
            else
                puzzboard[i][j]='#';
        }
    }
}

// show puzzle board
void showpuzzboard(char puzzboard[max+2][max+2]){
    printf("Crossword Puzzle:\n");
    for(int i= 0;  i<=max+1; i++){
        for(int j= 0; j<=max+1; j++){
            printf("%c", puzzboard[i][j]);
        }
        printf("\n");
    }
}

// store 2 longest words in the middle of the board on solution board
void updatesol(Cross arr[], char solboard[max+2][max+2], int *istart2, int *firstwordconnect, int *secondwordconnect){
    // (15)/2 + 1 = 8, middle of board
    // add longest word to middle
    int mid= 8; 
    int length= arr[0].length;
    int istart= (max+2 - length)/2;
    int letter1= 0;
    for(int i= istart; i<istart+length; i++){
        solboard[mid][i]= arr[0].word[letter1];
        letter1++;
    }
    // find place for second word to connect
    // first, iterate through each letter of second word
    int flag=1;
    while(flag){
        for(int j=0; j<strlen(arr[1].word); j++){
        // next, iterate through each letter of first word, seeing if it connects to each letter of the second
            for(int i= istart; i<istart+length; i++){
                if(solboard[mid][i] == arr[1].word[j]){
                    // make sure word fits on board
                    if(strlen(arr[1].word) <= j+mid){
                        *firstwordconnect= i;
                        *secondwordconnect= j;
                        flag=0;
                        break;
                    }
                }
            }
            if(flag==0)
                break;
        }
        // if match not found
        if(flag==1){
            printf("No match found!\n");
            break;
        }
    }
    arr[0].row=mid;
    arr[0].col= istart;
    // add second longest word to board
    int letter2=0;
    *istart2= mid-*secondwordconnect;
    for(int i= *istart2; i<strlen(arr[1].word)+*istart2; i++){
        solboard[i][*firstwordconnect]= arr[1].word[letter2];
        letter2++;
    }
    arr[1].row= *istart2;
    arr[1].col= *firstwordconnect;
}

// store longest word in the middle of the board on puzzle board
void updatepuzz(Cross arr[], char puzzboard[max+2][max+2], int *istart2, int *firstwordconnect, int *secondwordconnect){
    // (15)/2 + 1 = 8, middle of board
    int mid= 8; 
    int length= arr[0].length;
    int istart= (max+2 - length)/2;
    // first word
    for(int i= istart; i<istart+length; i++){
        puzzboard[mid][i]= ' ';

    }
    // second word
    for(int i= *istart2; i<strlen(arr[1].word)+*istart2; i++){
        puzzboard[i][*firstwordconnect]= ' ';
    }
}

// continue with the remaining words
int updatesol2(Cross arr[], char solboard[max+2][max+2], int count){
    int iword=1;
    int firstconnect, secondconnect;
    // counter for words that dont fit
    int xcount=0;
    // iterate through each word
    for(int i=0; i<count-2; i++){
        // flag to check is matching letter has been found and boundaries
        iword++;
        int flag=1;
        int boundflag=0;
        // iterate through letters of the word
        for(int j=0; j<arr[iword].length; j++){
            // iterate through each row of board        
            for(int row=0; row<max+1; row++){
                // iterate through each column of board
                for(int col=0; col<max+1; col++){
                    if(solboard[row][col] == arr[iword].word[j]){ 
                        firstconnect= col;
                        secondconnect= j;
                        // if word it is connecting to is HORIZONTAL
                        if(solboard[row-1][col]== '.' && solboard[row+1][col]== '.' ){
                            // reset bound checker
                            int boundflag=0;
                            // check boundary, make sure only going through matching letters and .s, or border if last 
                            for(int m=0; m<arr[iword].length; m++){
                                if(solboard[m+row-secondconnect][col]== '.' || solboard[m+row-secondconnect][col]== arr[iword].word[m] || (solboard[m+row-secondconnect][col]=='_' && m==arr[iword].length-1)){
                                    // check diagonal boundaries forward
                                    if(secondconnect==0){
                                        if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.')
                                            boundflag++;
                                        else if(solboard[m+row-secondconnect+1][col-1]== '_' && solboard[m+row-secondconnect+1][col+1]== '_' && m==arr[iword].length-1)
                                            boundflag++;
                                    }
                                    // check diagonal boundaries backward
                                    else if(secondconnect==arr[iword].length-1){
                                        if(solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.')
                                            boundflag++;
                                        else if(solboard[m+row-secondconnect-1][col-1]== '_' && solboard[m+row-secondconnect-1][col+1]== '_' && m==0)
                                            boundflag++;
                                    }
                                    // check diagonal boundaries both ways
                                    else if(secondconnect< arr[iword].length-1 && secondconnect>0){
                                        // if diagonals all clear
                                        if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.'){
                                            boundflag++;
                                        }
                                        // if diagonals touching bottom border
                                        else if(row==15 && solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.'){
                                            boundflag++;
                                        }
                                        // if diagnols touching top border
                                        else if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && row==1){
                                            boundflag++;
                                        }
                                        // if diagonals clear except for connecting word
                                        else if(m== secondconnect-1 || m== secondconnect+1){
                                            if(m==secondconnect-1){
                                                if(solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.' && solboard[m+row-secondconnect-1][col]== '.')
                                                    boundflag++;
                                            }
                                            else{
                                                if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && solboard[m+row-secondconnect+1][col]== '.')
                                                    boundflag++;
                                            }
                                                
                                        }
                                    }
                                        
                                    
                                }
                            }
                            // add word to board, along with direction, row, and column elements
                            if(boundflag==arr[iword].length){
                                for(int m=0; m<arr[iword].length; m++){
                                    solboard[m+row-secondconnect][col]= arr[iword].word[m];
                                }
                                strcpy(arr[iword].direction, "vertical");
                                arr[iword].row= row-secondconnect;
                                arr[iword].col= col;
                                arr[iword].fit= 1;
                                flag=0;
                            }
                        }
                        // if word it is connecting is VERTICAL
                        else if(solboard[row][col-1]== '.' && solboard[row][col+1]== '.' ){
                            // resest bound checker
                            int boundflag=0;
                            // check boundary, make sure only going through matching letters and .s, or border if last letter and that surrounding boxes clear
                            for(int n=0; n<arr[iword].length; n++){
                                if(solboard[row][n+col-secondconnect]== '.' || solboard[row][n+col-secondconnect]== arr[iword].word[n] || (solboard[row][n+col-secondconnect]== '|' && n==arr[iword].length-1) || (solboard[row][n+col-secondconnect]== '|' && n==0)){
                                    // check diagonal spaces to side
                                    if(secondconnect==0){
                                        if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' )
                                            boundflag++;
                                        else if(n==arr[iword].length-1 && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect+1]== '.')
                                            boundflag++;
                                    }
                                    // check diagonal spaces to other side
                                    else if(secondconnect==arr[iword].length-1){
                                        if(solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.'){
                                            boundflag++;
                                        }
                                        else if(n==0 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.'){
                                            boundflag++;
                                        }
                                        else if(n==0 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.'){
                                            boundflag++;
                                        }
                                    }
                                    // check diagonal spaces both ways
                                    else if ((secondconnect< arr[iword].length-1) && (secondconnect>0)){
                                        // if all diagonals are clear
                                        if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect+1]== '.'){
                                            boundflag++;
                                        }
                                        // if touching right side of border
                                        else if(col==15 && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' &&  n==arr[iword].length-1){                                      
                                            boundflag++;
                                        }
                                        // if touching left side of border
                                        else if(col==1 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' &&  n==arr[iword].length-1){                                        
                                            boundflag++;
                                        }
                                        // if touching top border
                                        else if(row==1 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row+1][n+col-secondconnect-1]== '.'){
                                            boundflag++;
                                        }
                                        // if touching bottom border
                                        else if(row==15 && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.'){
                                            boundflag++;
                                        }
                                        // if diagonal are clear except one is from connecting word
                                        else if(n== secondconnect-1 || n== secondconnect+1){
                                            if(n==secondconnect-1){
                                                if(solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect-1]== '.'){
                                                    boundflag++;
                                                }
                                            }
                                            else{
                                                if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row][n+col-secondconnect+1]== '.'){
                                                    boundflag++;
                                                }
                                            }
                                                
                                        }
                                    }
                                }
                            }
                            // add word to board and direction, row, and column elements
                            if(boundflag==arr[iword].length){
                                for(int n=0; n<arr[iword].length; n++){
                                    solboard[row][n+col-secondconnect]= arr[iword].word[n];
                                }
                                strcpy(arr[iword].direction, "horizontal");
                                arr[iword].row= row;
                                arr[iword].col= col-secondconnect;
                                arr[iword].fit= 1;
                                flag=0;
                            }
                        }
                    }
                    // leave for loop if good match found
                    if(flag==0)
                        break;
                }
            }
            // leave for loop if good match found
            if(flag==0){
                break;
            }
        } 
        // FIRST EXTRA CREDIT (5 points) if word doesn't fit
        if(flag==1){
            printf("%s could not fit on the board on the first try!\n", arr[iword].word);
            xcount++;
            arr[iword].fit=0;
        }
    }
    printf("\n");
    return xcount;
}

// update puzzle board with reamining words
void updatepuzz2(Cross arr[], char puzzboard[max+2][max+2], char solboard[max+2][max+2]){
    for(int i=1; i<max+1; i++){
        for(int j=1; j<max+1; j++){
            if(solboard[i][j]== '.'){
                puzzboard[i][j]= '#';
            }
            else{
                puzzboard[i][j]= ' ';
            }
            
        }
    }
}

// display anagram clues for crossword
void clues(Cross arr[], int count){
    printf("Clues:\n");
    printf("Location | Direction   | Anagram\n");
    for(int i=0; i<count; i++){
        if(arr[i].row>0 && arr[i].row<=max){
            printf("%2d, %2d   |%11s  | %3s\n", arr[i].col, arr[i].row, arr[i].direction, strfry(arr[i].word));
        }
    }
}

// read in words from file
int readwordsfile(FILE *fp, char words[][max+1]){
    int count=0;
    char newword[max];
    // while word count hasn't reached 20 or . been typed
    while(1){
        // scan in words from file
        fscanf(fp,"%s", &newword);
        // stop if '.' is typed
        if((newword[0])== '.')
            break;
        // check validity of word
        bool B= true;
        // flag to mark if work contains non-letters or too long/short
        int flag=1;
        while(B){
            flag=1;
            // check if word is too long
            if(strlen(newword)>=16){
                printf("Error, word too long\n");
                B= false;
                flag=0;
            }
            // check if word is too short
            else if(strlen(newword)<2){
                printf("Error, word too short\n");
                B= false;
                flag=0;
            }
            // check if word contains non-letters
            else{

                for(int i=0; i<strlen(newword);i++){
                 if(!isalpha(newword[i])){
                    printf("Error, cannot except non-letters\n");
                    B= false;
                    flag=0;
                    }
                }
            }
            // if word is good size and has all letters a-z (checked by flag)
            if(flag==1){
                strcpy(words[count], newword);
                count++;
                B= false;
            }
        }
        // while word count hasn't reached 20
        if(count>=20){
            break;
        }
    }
    // once words 2D array made, change to all upper by indexing through each letter ERROR BEFORE HERE
    for(int i=0; i<count; i++){
        for(int j=0; j<sizeof(words[i]); j++){
            words[i][j]= toupper(words[i][j]);
        }
    }
    return count;
}

void wordsintofile(Cross arr[], char words[][max+1],char solboard[max+2][max+2],char puzzboard[max+2][max+2], int count, char newfile[30]){
    FILE *fp= fopen(newfile, "w");
    for(int i=0; i<count; i++){
        fputs(arr[i].word, fp);
        fputs("\n", fp);
    }
    
    // print solution board
    fputs(" Solution:\n", fp);
    for(int i= 0;  i<=max+1; i++){
        for(int j= 0; j<=max+1; j++){
            fputc(solboard[i][j], fp);
        }
        fputs("\n", fp);
    }

    // print puzzle board
    fputs("Crossword Puzzle:\n", fp);
    for(int i= 0;  i<=max+1; i++){
        for(int j= 0; j<=max+1; j++){
            fputc(puzzboard[i][j], fp);
        }
        fputs("\n", fp);
    }

    // print clues
    fputs("Clues:\n", fp);
    fputs("Location | Direction   | Anagram\n", fp);
    for(int i=0; i<count; i++){
        if(arr[i].row>0 && arr[i].row<=max){
            fprintf(fp, "%2d, %2d   |%11s  | %3s\n", arr[i].col, arr[i].row, arr[i].direction, strfry(arr[i].word));
        }
    }
    fclose(fp);
}

// organize words by descending length and store in struct, then print into output file
void organizewordsintofile(char words[][max+1], Cross arr[], int count, char newfile[30]){
    FILE *fp= fopen(newfile, "w");
    int k=0;
    // go by descending length order from max down, then go through words array
    for(int i=15; i>0; i--){
        for(int j=0; j<count; j++){
            if(strlen(words[j])==i){
                // store both word and length in struct
                strcpy(arr[k].word, words[j]);
                arr[k].length = i;
                k++;
            }
        }   
    }
    strcpy(arr[0].direction, "horizontal");
    strcpy(arr[1].direction, "vertical");
    // print words in descending length order
    fputs("\n", fp);
    for(int i=0; i<count; i++){
        fprintf(fp, "%s\n", arr[i].word);
    }
}

// continue with the remaining words for 3 argument on command line
void updatesol2intofile(Cross arr[], char solboard[max+2][max+2], int count, char newfile[30]){
    FILE *fp= fopen(newfile, "w");
    int iword=1;
    int firstconnect, secondconnect;
    // iterate through each word
    for(int i=0; i<count-2; i++){
        // flag to check is matching letter has been found and boundaries
        iword++;
        int flag=1;
        int boundflag=0;
        // flag to check for words that dont fit
        int xcount=0;
        // iterate through letters of the word
        for(int j=0; j<arr[iword].length; j++){
            // iterate through each row of board        
            for(int row=0; row<max+1; row++){
                // iterate through each column of board
                for(int col=0; col<max+1; col++){
                    if(solboard[row][col] == arr[iword].word[j]){ 
                        firstconnect= col;
                        secondconnect= j;
                        // if word it is connecting to is HORIZONTAL
                        if(solboard[row-1][col]== '.' && solboard[row+1][col]== '.' ){
                            // reset bound checker
                            int boundflag=0;
                            // check boundary, make sure only going through matching letters and .s, or border if last 
                            for(int m=0; m<arr[iword].length; m++){
                                if(solboard[m+row-secondconnect][col]== '.' || solboard[m+row-secondconnect][col]== arr[iword].word[m] || (solboard[m+row-secondconnect][col]=='_' && m==arr[iword].length-1)){
                                    // check diagonal boundaries forward
                                    if(secondconnect==0){
                                        if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.')
                                            boundflag++;
                                        else if(solboard[m+row-secondconnect+1][col-1]== '_' && solboard[m+row-secondconnect+1][col+1]== '_' && m==arr[iword].length-1)
                                            boundflag++;
                                    }
                                    // check diagonal boundaries backward
                                    else if(secondconnect==arr[iword].length-1){
                                        if(solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.')
                                            boundflag++;
                                        else if(solboard[m+row-secondconnect-1][col-1]== '_' && solboard[m+row-secondconnect-1][col+1]== '_' && m==0)
                                            boundflag++;
                                    }
                                    // check diagonal boundaries both ways
                                    else if(secondconnect< arr[iword].length-1 && secondconnect>0){
                                        // if diagonals all clear
                                        if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.'){
                                            boundflag++;
                                        }
                                        // if diagonals touching bottom border
                                        else if(row==15 && solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.'){
                                            boundflag++;
                                        }
                                        // if diagnols touching top border
                                        else if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && row==1){
                                            boundflag++;
                                        }
                                        // if diagonals clear except for connecting word
                                        else if(m== secondconnect-1 || m== secondconnect+1){
                                            if(m==secondconnect-1){
                                                if(solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.' && solboard[m+row-secondconnect-1][col]== '.')
                                                    boundflag++;
                                            }
                                            else{
                                                if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && solboard[m+row-secondconnect+1][col]== '.')
                                                    boundflag++;
                                            }
                                                
                                        }
                                    }
                                        
                                    
                                }
                            }
                            // add word to board, along with direction, row, and column elements
                            if(boundflag==arr[iword].length){
                                for(int m=0; m<arr[iword].length; m++){
                                    solboard[m+row-secondconnect][col]= arr[iword].word[m];
                                }
                                strcpy(arr[iword].direction, "vertical");
                                arr[iword].row= row-secondconnect;
                                arr[iword].col= col;
                                arr[iword].fit= 1;
                                flag=0;
                            }
                        }
                        // if word it is connecting is VERTICAL
                        else if(solboard[row][col-1]== '.' && solboard[row][col+1]== '.' ){
                            // resest bound checker
                            int boundflag=0;
                            // check boundary, make sure only going through matching letters and .s, or border if last letter and that surrounding boxes clear
                            for(int n=0; n<arr[iword].length; n++){
                                if(solboard[row][n+col-secondconnect]== '.' || solboard[row][n+col-secondconnect]== arr[iword].word[n] || (solboard[row][n+col-secondconnect]== '|' && n==arr[iword].length-1) || (solboard[row][n+col-secondconnect]== '|' && n==0)){
                                    // check diagonal spaces to side
                                    if(secondconnect==0){
                                        if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' )
                                            boundflag++;
                                        else if(n==arr[iword].length-1 && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect+1]== '.')
                                            boundflag++;
                                    }
                                    // check diagonal spaces to other side
                                    else if(secondconnect==arr[iword].length-1){
                                        if(solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.'){
                                            boundflag++;
                                        }
                                        else if(n==0 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.'){
                                            boundflag++;
                                        }
                                        else if(n==0 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.'){
                                            boundflag++;
                                        }
                                    }
                                    // check diagonal spaces both ways
                                    else if ((secondconnect< arr[iword].length-1) && (secondconnect>0)){
                                        // if all diagonals are clear
                                        if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect+1]== '.'){
                                            boundflag++;
                                        }
                                        // if touching right side of border
                                        else if(col==15 && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' &&  n==arr[iword].length-1){                                      
                                            boundflag++;
                                        }
                                        // if touching left side of border
                                        else if(col==1 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' &&  n==arr[iword].length-1){                                        
                                            boundflag++;
                                        }
                                        // if touching top border
                                        else if(row==1 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row+1][n+col-secondconnect-1]== '.'){
                                            boundflag++;
                                        }
                                        // if touching bottom border
                                        else if(row==15 && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.'){
                                            boundflag++;
                                        }
                                        // if diagonal are clear except one is from connecting word
                                        else if(n== secondconnect-1 || n== secondconnect+1){
                                            if(n==secondconnect-1){
                                                if(solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect-1]== '.'){
                                                    boundflag++;
                                                }
                                            }
                                            else{
                                                if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row][n+col-secondconnect+1]== '.'){
                                                    boundflag++;
                                                }
                                            }
                                                
                                        }
                                    }
                                }
                            }
                            // add word to board and direction, row, and column elements
                            if(boundflag==arr[iword].length){
                                for(int n=0; n<arr[iword].length; n++){
                                    solboard[row][n+col-secondconnect]= arr[iword].word[n];
                                }
                                strcpy(arr[iword].direction, "horizontal");
                                arr[iword].row= row;
                                arr[iword].col= col-secondconnect;
                                arr[iword].fit= 1;
                                flag=0;
                            }
                        }
                    }
                    // leave for loop if good match found
                    if(flag==0)
                        break;
                }
            }
            // leave for loop if good match found
            if(flag==0){
                break;
            }
        } 
        // FIRST EXTRA CREDIT (5 points) if words doesnt fit
        if(flag==1){
            fprintf(fp, "%s could not fit on board!\n", arr[iword].word);
            xcount++;
        }
    }
}

// try to fit in words that did not fit the first time
void updatesol3(Cross arr[], char solboard[max+2][max+2], int count, int xcount){
    int iword=1;
    int firstconnect, secondconnect;
    // iterate through each word
    for(int i=0; i<count-2; i++){
        // flag to check is matching letter has been found and boundaries
        iword++;
        int flag=1;
        int boundflag=0;
        // iterate through letters of the word
        for(int j=0; j<arr[iword].length; j++){
            // if word did not fit first time 
            if(arr[iword].fit==0){
                // iterate through each row of board        
                for(int row=0; row<max+1; row++){
                    // iterate through each column of board
                    for(int col=0; col<max+1; col++){
                        if(solboard[row][col] == arr[iword].word[j]){ 
                            firstconnect= col;
                            secondconnect= j;
                            // if word it is connecting to is HORIZONTAL
                            if(solboard[row-1][col]== '.' && solboard[row+1][col]== '.' ){
                                // reset bound checker
                                int boundflag=0;
                                // check boundary, make sure only going through matching letters and .s, or border if last 
                                for(int m=0; m<arr[iword].length; m++){
                                    if(solboard[m+row-secondconnect][col]== '.' || solboard[m+row-secondconnect][col]== arr[iword].word[m] || (solboard[m+row-secondconnect][col]=='_' && m==arr[iword].length-1)){
                                        // check diagonal boundaries forward
                                        if(secondconnect==0){
                                            if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.')
                                                boundflag++;
                                            else if(solboard[m+row-secondconnect+1][col-1]== '_' && solboard[m+row-secondconnect+1][col+1]== '_' && m==arr[iword].length-1)
                                                boundflag++;
                                        }
                                        // check diagonal boundaries backward
                                        else if(secondconnect==arr[iword].length-1){
                                            if(solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.')
                                                boundflag++;
                                            else if(solboard[m+row-secondconnect-1][col-1]== '_' && solboard[m+row-secondconnect-1][col+1]== '_' && m==0)
                                                boundflag++;
                                        }
                                        // check diagonal boundaries both ways
                                        else if(secondconnect< arr[iword].length-1 && secondconnect>0){
                                            // if diagonals all clear
                                            if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.'){
                                                boundflag++;
                                            }
                                            // if diagonals touching bottom border
                                            else if(row==15 && solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.'){
                                                boundflag++;
                                            }
                                            // if diagnols touching top border
                                            else if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && row==1){
                                                boundflag++;
                                            }
                                            // if diagonals clear except for connecting word
                                            else if(m== secondconnect-1 || m== secondconnect+1){
                                                if(m==secondconnect-1){
                                                    if(solboard[m+row-secondconnect-1][col-1]== '.' && solboard[m+row-secondconnect-1][col+1]== '.' && solboard[m+row-secondconnect-1][col]== '.')
                                                        boundflag++;
                                                }
                                                else{
                                                    if(solboard[m+row-secondconnect+1][col-1]== '.' && solboard[m+row-secondconnect+1][col+1]== '.' && solboard[m+row-secondconnect+1][col]== '.')
                                                        boundflag++;
                                                }
                                                
                                            }
                                        }
                                        
                                    
                                    }
                                }
                                // add word to board, along with direction, row, and column elements
                                if(boundflag==arr[iword].length){
                                    for(int m=0; m<arr[iword].length; m++){
                                        solboard[m+row-secondconnect][col]= arr[iword].word[m];
                                    }
                                    strcpy(arr[iword].direction, "vertical");
                                    arr[iword].row= row-secondconnect;
                                    arr[iword].col= col;
                                    arr[iword].fit= 1;
                                    flag=0;
                                }
                            }
                            // if word it is connecting is VERTICAL
                            else if(solboard[row][col-1]== '.' && solboard[row][col+1]== '.' ){
                                // resest bound checker
                                int boundflag=0;
                                // check boundary, make sure only going through matching letters and .s, or border if last letter and that surrounding boxes clear
                                for(int n=0; n<arr[iword].length; n++){
                                    if(solboard[row][n+col-secondconnect]== '.' || solboard[row][n+col-secondconnect]== arr[iword].word[n] || (solboard[row][n+col-secondconnect]== '|' && n==arr[iword].length-1) || (solboard[row][n+col-secondconnect]== '|' && n==0)){
                                        // check diagonal spaces to side
                                        if(secondconnect==0){
                                            if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' )
                                                boundflag++;
                                            else if(n==arr[iword].length-1 && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect+1]== '.')
                                                boundflag++;
                                        }
                                        // check diagonal spaces to other side
                                        else if(secondconnect==arr[iword].length-1){
                                            if(solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.'){
                                                boundflag++;
                                            }
                                            else if(n==0 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.'){
                                                boundflag++;
                                            }
                                            else if(n==0 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.'){
                                                boundflag++;
                                            }
                                        }
                                        // check diagonal spaces both ways
                                        else if ((secondconnect< arr[iword].length-1) && (secondconnect>0)){
                                            // if all diagonals are clear
                                            if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect+1]== '.'){
                                                boundflag++;
                                            }
                                            // if touching right side of border
                                            else if(col==15 && solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' &&  n==arr[iword].length-1){                                      
                                                boundflag++;
                                            }
                                            // if touching left side of border
                                            else if(col==1 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' &&  n==arr[iword].length-1){                                        
                                                boundflag++;
                                            }
                                            // if touching top border
                                            else if(row==1 && solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row+1][n+col-secondconnect-1]== '.'){
                                                boundflag++;
                                            }
                                            // if touching bottom border
                                            else if(row==15 && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.'){
                                                boundflag++;
                                            }
                                            // if diagonal are clear except one is from connecting word
                                            else if(n== secondconnect-1 || n== secondconnect+1){
                                                if(n==secondconnect-1){
                                                    if(solboard[row+1][n+col-secondconnect-1]== '.' && solboard[row-1][n+col-secondconnect-1]== '.' && solboard[row][n+col-secondconnect-1]== '.'){
                                                        boundflag++;
                                                    }
                                                }
                                                else{
                                                    if(solboard[row+1][n+col-secondconnect+1]== '.' && solboard[row-1][n+col-secondconnect+1]== '.' && solboard[row][n+col-secondconnect+1]== '.'){
                                                        boundflag++;
                                                    }
                                                }
                                                
                                            }
                                        }
                                    }
                                }
                                // add word to board and direction, row, and column elements
                                if(boundflag==arr[iword].length){
                                    for(int n=0; n<arr[iword].length; n++){
                                        solboard[row][n+col-secondconnect]= arr[iword].word[n];
                                    }
                                    strcpy(arr[iword].direction, "horizontal");
                                    arr[iword].row= row;
                                    arr[iword].col= col-secondconnect;
                                    arr[iword].fit= 1;
                                    flag=0;
                                }
                            }
                        }
                        // leave for loop if good match found
                        if(flag==0)
                            break;
                    }
                }
                // leave for loop if good match found
                if(flag==0){
                    break;
                }
            }
            else{
                flag=0;
            } 
        }
        // SECOND EXTRA CREDIT (10 points) if word doesn't fit AGAIN
        if(flag==1 && arr[iword].fit== 0){
            printf("%s could not fit on board!\n", arr[iword].word);
        }
        printf("\n");
    }
}
