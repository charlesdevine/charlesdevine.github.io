// Charlie Devine, 12/5/23, Fund Comp Lab 11, Mini Final Project
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "gfx.h"

// FUNCTION PROTOTYPES
typedef struct{
    int x;
    int y;
    int points;
    char direction[10];
} Block;
void makerectangle(int x, int y, int width, int height);
void intro(int width, int height, int blocksize);
void draw(float *size, int blocksize, Block block, int width, int height, int *fx, int *fy, int *red, int *blue, int highscore);
void move(Block *block, int width, int height, int blocksize, float *size,float *speed, int *flagintro, int *wanttoquit);
int collide(Block *block, int width, int height, int blocksize, int *fx, int *fy, float *size, float *speed, int *red, int *blue);
int gethighscore();
void updatehighscore(int highscore);
void displaypoints(Block *block, int highscore);

// MAIN SCRIPT
int main(){
    Block block;
    // scale of game in reference to a 10x10 board
    int scale=5;
    // dimensions of window
    int width= scale*100, height=scale*100;
    // size of one unit of block/food
    int blocksize= scale*10;
    // initial block location
    block.x= blocksize*4, block.y=blocksize*2;
    // initial block size
    float size= 1;
    // changes in dx and dy
    int dx=scale*10, dy=scale*10;
    // check if wall is hit
    int ifwallhit= 0;
    // intial block speed
    float speed= 0.05;
    // intial block red color
    int red= 66;
    // initial food blue color
    int blue= 50;

    // open game window
    gfx_open(width,height,"Block.io");

    // start game with 0 points 
    block.points=0;

    // get high score
    int highscore= gethighscore();

    // intial food location
    int fx= rand() % (width/blocksize) * blocksize;
    int fy= rand() % (height/blocksize) * blocksize;

    // check if keys have been clicked
    int flagintro=0;

    // check if user has clicked q
    int wanttoquit=0;

    while(1){
        // generate different seed every time
        srand(time(0));

        // game introduction if no keys/mouse have been clicked
        if(flagintro==0){
            intro(width, height, blocksize);
        }

        // if q is clicked, quit the game
        if(wanttoquit==1){
            break;
        }
        // background color
        gfx_clear_color(207,234,203);

        // if the wall gets hit, game over
        if(ifwallhit== 1){
            gfx_color(0,0,0);
            gfx_text(width/2-50,400, "GAME OVER");
            // if you beat high score, you win
            // if tied, display that
            if((block.points>highscore)){
                gfx_color(0,200,0);
                char highscorestring[3];
                sprintf(highscorestring, "NEW HIGH SCORE: %d", block.points);
                gfx_text(width/2-50,415, highscorestring);
                gfx_text(width/2-50,430, "YOU WIN!");
                highscore= block.points;
                updatehighscore(highscore);
                if (gfx_event_waiting()) {
                    char d;
                    d = gfx_wait();
                    if(d=='q'){
                        wanttoquit=1;
                    }
                }
            }
            // if high score isn't beat, you lose
            else if(block.points<highscore){
                gfx_color(200,0,0);
                gfx_text(width/2-50,415, "YOU LOSE!");
                if (gfx_event_waiting()) {
                    char d;
                    d = gfx_wait();
                    if(d=='q'){
                        wanttoquit=1;
                    }
                }
            }
        }
        // otherwise, play the game
        else{
            // make and move the block
            draw(&size,blocksize,block,width,height,&fx,&fy,&red,&blue,highscore);
            move(&block,width,height,blocksize,&size,&speed,&flagintro,&wanttoquit);
            ifwallhit= collide(&block,width,height,blocksize,&fx,&fy,&size,&speed,&red, &blue);
        }
    }


    return 0;
}

// FUNCION DEFINTIONS   
// make the shape of a rectangle for the block and food
void makerectangle(int x, int y, int width, int height){
    for(int i=0; i<height; i++){
        gfx_line(x, y+i, x+width-1, y+i);
    }
}

// game introduction
void intro(int width, int height, int blocksize){
    gfx_color(0,0,0);
    gfx_text(width / 2 - 100, height / 3, "Welcome to Block.io!");
    gfx_text(width / 2 - 200, height / 3+15, "To play this game, you will use the a, d, s, w keys");
    gfx_text(width / 2 - 220, height / 3+25, "to direct your blue block (west, east, south, north respectively).");
    gfx_text(width / 2 - 157, height / 3+35, "Use these keys and try to eat the red food.");
    gfx_text(width / 2 - 200, height / 3+45, "Each time you eat the food, your block will grow");
    gfx_text(width / 2 - 200, height / 3+55, "and for every few points gained, it will speed up.");
    gfx_text(width / 2 - 240, height / 3+65, "Make sure to avoid hitting the wall and grow as much as possible to win!");
    gfx_text(width / 2 - 240, height / 3+75, "First click this window, then click any of the a, d, s, w keys to begin.");
    gfx_text(width / 2 - 210, height / 3+85, "Click the q key whenever you would like to quit the program.");
}


// draw on the board the block and the food
void draw(float *size, int blocksize, Block block, int width, int height, int *fx, int *fy, int *red,int *blue, int highscore){
    gfx_clear();
    // draw a blue block
    gfx_color(*red,97,143);
    makerectangle(block.x,block.y,blocksize*(*size),blocksize*(*size));
    // draw a red rectangle for food
    gfx_color(255,0,*blue);
    makerectangle(*fx,*fy,blocksize,blocksize);

    displaypoints(&block,highscore);

    gfx_flush();
}

// function that moves the block around board
void move(Block *block, int width, int height, int blocksize, float *size, float *speed, int *flagintro, int *wanttoquit) {
    if (gfx_event_waiting()) {
        char c;
        c = gfx_wait();
        *flagintro=1;
        // click the respective key for your desired direction
        if (c == 'a' && (*block).x > 0) {
            strcpy((*block).direction, "left");
        }
        if (c == 'd' && (*block).x < width-blocksize*(*size)) {
            strcpy((*block).direction, "right");
        }
        if (c == 'w' && (*block).y > 0) {
            strcpy((*block).direction, "up");
        }
        if (c == 's' && (*block).y < height-blocksize*(*size)) {
            strcpy((*block).direction, "down");
        }
        // quit if user clicks q
        if (c== 'q'){
            *wanttoquit=1;
        }
        gfx_flush();
    }
    // if any event is not waiting, constantly move in current direction
    else{
        if(strcmp((*block).direction, "left") == 0 && (*block).x>0){
            (*block).x -= (blocksize*(*speed));
        }
        if(strcmp((*block).direction, "right") == 0 && (*block).x<width-blocksize){
            (*block).x += (blocksize*(*speed));
        }
        if(strcmp((*block).direction, "up") == 0 && (*block).y>0){
            (*block).y -= (blocksize*(*speed));
        }
        if(strcmp((*block).direction, "down") == 0 && (*block).y<height-blocksize){
            (*block).y += (blocksize*(*speed));
        }
    }
}

// check if block and food collided
int collide(Block *block, int width, int height, int blocksize, int *fx, int *fy, float *size, float *speed, int *red, int *blue){
    // if blue and red collide increase size
    int wallhit;
    if((*block).x+blocksize*(*size)> *fx && (*block).x< *fx+blocksize && (*block).y + blocksize*(*size)> *fy  && (*block).y < *fy+blocksize){
        // intial food location
        *fx= rand() % (width/blocksize) * blocksize;
        *fy= rand() % (height/blocksize) * blocksize;
        // make sure new randomized food location is not still touching blue
        int ifstilltouching=1;
        while(ifstilltouching){
            if((*block).x+blocksize*(*size)> *fx && (*block).x< *fx+blocksize*(*size) && (*block).y + blocksize*(*size)> *fy  && (*block).y < *fy+blocksize*(*size)){
                *fx= rand() % (width/blocksize) * blocksize;
                *fy= rand() % (height/blocksize) * blocksize;
            }
            else{
                ifstilltouching=0;
                break;
            }
        }
        // increase size, points, and change color when block collides with food
        (*size) += 0.075;
        (*block).points++;
        (*blue) += 1;
    }
    // add speed as more points are gained, for every 4 points
    if((*block).points%3==0 && (*block).points != 0){
        (*speed) += 0.0002;
        *red += 10;
        gfx_text( width/2-50, height/6,"Speed Increase!");
    }

    // if the red collides with the wall
    if((*block).x<=0 || (*block).x+blocksize*(*size)>=width || (*block).y<=0 || (*block).y+blocksize*(*size)>=height){
        gfx_color(0,0,0);
        gfx_text(width/2-50,385, "Wall Hit!");
        wallhit=1;
        return wallhit;
    }
}

// get high school from file
int gethighscore(){
    FILE *file =fopen("highscore.txt", "r");
    int highscore=0;
    if(file != NULL){
        fscanf(file, "%d", &highscore);
        fclose(file);
    }
    return highscore;
}

// update high score
void updatehighscore(int highscore){
    FILE *file =fopen("highscore.txt", "w");
    if(file != NULL){
        fprintf(file, "%d", highscore);
        fclose(file);
    }
}

// display points and high score in top right corner
void displaypoints(Block *block, int highscore){
    gfx_color(0,0,0);
    // Display points
    char points_str[20];
    sprintf(points_str, "Points: %d", (*block).points);
    gfx_text(10,20,points_str);
    // Display high score
    char highscore_str[4];
    sprintf(highscore_str, "Current High Score: %d", highscore);
    gfx_text(10,35,highscore_str);
}
