// Charlie Devine, 11/2/23, Fund Comp Lab 8, Crossword Header
#include <stdio.h>
#include <string.h>
#define max 15

//make struct
typedef struct{
    char word[max+1];
    int length;
    char direction[max];
    int row;
    int col;
    int fit;
}   Cross;

// function prototypes
// read in worlda and organize
int readwords(char[21][max+1]);
void organizewords(char[21][max+1], Cross arr[max+1], int count);

// make both boards
void makesolboard(char solboard[max+2][max+2]);
void showsolboard(char solboard[max+2][max+2]);
void makepuzzboard(char puzzboard[max+2][max+2]);
void showpuzzboard(char puzzboard[max+2][max+2]);

// add first and second words to board
void updatesol(Cross arr[max], char solboard[max+2][max+2], int *, int *, int *);
void updatepuzz(Cross arr[max], char puzzboard[max+2][max+2], int *, int *, int *);

// add remaining words
int updatesol2(Cross arr[max], char solboard[max+2][max+2], int count);
void updatepuzz2(Cross arr[], char puzzboard[max+2][max+2], char solboard[max+2][max+2]);

// clues
void clues(Cross arr[], int count);

// same functions but for files
int readwordsfile(FILE *, char[21][max+1]);
void wordsintofile(Cross arr[], char words[][max+1],char solboard[max+2][max+2],char puzzboard[max+2][max+2], int count, char newfile[30]);
void organizewordsintofile(char words[][max+1], Cross arr[], int count, char newfile[30]);
void updatesol2intofile(Cross arr[], char solboard[max+2][max+2], int count, char newfile[30]);

// second extra credit- try to add words that didnt fit first time
void updatesol3(Cross arr[], char solboard[max+2][max+2], int count, int xcount);