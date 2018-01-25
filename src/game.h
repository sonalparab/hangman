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

#ifndef GAME_H
#define GAME_H
//create the guessing array with '_' for the
// letters based on the length of the word
char * blank_array(int);

//generate hangman based on the number of
// wrong guesses
char * generate_man(int);

//run the game
void run_game(char *, int);

#endif
