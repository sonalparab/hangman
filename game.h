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

#define PROMPT "Pick a letter: "

char * blank_array(int);

void run_game(char *, int, int);

char * generate_man(int);
