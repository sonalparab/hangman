#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>

#define MODE_PROMPT "Pick a game mode: "
#define GAME_PROMPT "Pick a letter: "

#ifndef COMPETITIVE_GAME_H
#define COMPETITIVE_GAME_H

//used for second player when one player goes multiple times
// in a row
void get_status(int, int);

//run one turn
int run_turn_competitive(int, int *, char *, char *, int *, char *, int);

//run the game for competitive mode
void run_game_competitive(char *, int);

#endif
