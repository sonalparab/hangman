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

#ifndef COLLAB_GAME_H
#define COLLAB_GAME_H

//run one turn
int run_turn_collab(int, int *, char *, char *, int *, char *, int);

//run the game for collab mode
void run_game_collab(char *, int);

#endif
