// Charlie Devine, 11/2/23, Fund Comp Lab 8, Crossword Main
#include <stdio.h>
#include <string.h>
#include "crossfunc.h"

int main(int argc, char*argv[]){
    Cross arr[max+1];
    char words[20][max+1];
    char solboard[max+2][max+2];
    char puzzboard[max+2][max+2];
    int count, xcount;
    int istart2, firstwordconnect, secondwordconnect;
    char newfile[30];

    // determine game mode
    // if only one command line argument
    if(argc==1){
        // read in words from keyboard and organize by length
        count=readwords(words);
        organizewords(words, arr, count);
        printf("\n");

        // create blank solution board and puzzle board
        makesolboard(solboard);
        makepuzzboard(puzzboard);

        // add first and second words
        updatesol(arr, solboard, &istart2, &firstwordconnect, &secondwordconnect);
        updatepuzz(arr, puzzboard, &istart2, &firstwordconnect, &secondwordconnect);

        // add remaining words
        updatesol2(arr, solboard, count);
        updatesol3(arr, solboard, count, xcount);
        showsolboard(solboard);
        updatepuzz2(arr, puzzboard, solboard);
        showpuzzboard(puzzboard);

        // clues
        clues(arr, count);
    }
    // if two command line arguments
    else if(argc==2){
        //open file from command line
        const char *file=argv[1];
        FILE *fp= fopen(file,"r");
        if(!fp){
            printf("File not found!\n");
            return 1;
        }
        // read in words from file and organize by length
        count=readwordsfile(fp, words);
        organizewords(words, arr, count);
        printf("\n");

        // create blank solution board and puzzle board
        makesolboard(solboard);
        makepuzzboard(puzzboard);

        // add first and second words
        updatesol(arr, solboard, &istart2, &firstwordconnect, &secondwordconnect);
        updatepuzz(arr, puzzboard, &istart2, &firstwordconnect, &secondwordconnect);

        // add remaining words
        xcount= updatesol2(arr, solboard, count);
        showsolboard(solboard);
        updatepuzz2(arr, puzzboard, solboard);
        showpuzzboard(puzzboard);

        // clues
        clues(arr, count);

    }
    // if three comand line arguments
    else if(argc==3){
        //open file from command line
        const char *file=argv[1];
        FILE *fp= fopen(file,"r");
        if(!fp){
            printf("File not found!\n");
            return 1;
        }

        // copy third argument as output file
        strcpy(newfile, argv[2]);

        // read in words from file and organize by length
        count=readwordsfile(fp, words);
        organizewordsintofile(words, arr, count, newfile);
        printf("\n");

        // create blank solution board and puzzle board
        makesolboard(solboard);
        makepuzzboard(puzzboard);

        // add first and second words
        updatesol(arr, solboard, &istart2, &firstwordconnect, &secondwordconnect);
        updatepuzz(arr, puzzboard, &istart2, &firstwordconnect, &secondwordconnect);

        // add remaining words
        updatesol2intofile(arr, solboard, count, newfile);
        updatepuzz2(arr, puzzboard, solboard);
        wordsintofile(arr, words, solboard, puzzboard, count, newfile);
    }
    // if too many arguments entered
    else{
        printf("Error, too many arguments entered!\n");
    }

    return 0;
}


