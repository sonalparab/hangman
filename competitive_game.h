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

void get_status(int, int, int);

int run_turn_competitive(int, int *, char *, char *, int *, char *, int, int);

void run_game_competitive(char *, int, int);

#endif
