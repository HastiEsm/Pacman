#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <termios.h>

#define RED "\x1b[38;2;255;0;0m"
#define GREEN "\x1b[38;2;0;255;0m"
#define YELLOW "\x1b[38;2;255;255;0m"
#define BLUE "\x1b[38;2;0;0;255m"
#define MAGENTA "\x1b[38;2;225;25;225m"
#define CYAN "\x1b[38;2;0;255;255m"
#define WHITE "\x1b[38;2;255;255;255m"
#define GRAY "\x1b[38;2;100;100;100m"
#define ORANGE "\x1b[38;2;255;165;0m"
#define PINK "\x1b[38;2;255;100;190m"

typedef struct mapinfo
{
    char **map;
    int x;
    int y;
} mapinfo;

typedef struct account
{
    char id[20];
    int level;
    int lastGame;
    int score;
    char filename[100];
    int password;
    int valid;
} account;

typedef struct coordinate
{
    int i;
    int j;
} coordinate;

mapinfo load(int);
/*
Description: Loads the game map based on the selected level
Output: Game map based on the selected level
Input: selected level
*/
account loadAcc(char *, mapinfo *, int *);
/*
Description: Loads player account information
Output: Player account information
Input: Player id, game map, game level
*/
void playA(mapinfo *, account *, int *);
/*
Description: Main game loop for level 1
Input: Game map, player account information, time
*/
void playB(mapinfo *, account *, int, int *);
/*
Description: Main game loop for levels 2 & 3
Input: Game map, player account information, game level, time
*/
void print(mapinfo *, account *);
/*
Description: Prints the game board
Input: Game map, player account information
*/
void store(mapinfo *, account *);
/*
Description: Stores player data after the game
Input: Game map, player account information
*/

#endif
