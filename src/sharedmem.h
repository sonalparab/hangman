#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

//used for collab mode
#define WRONG_GUESSES_KEY 850
#define G_KEY 851
#define GUESSING_ARRAY_KEY 852
#define GUESSED_LETTER_KEY 853

//used for competitive mode
#define WRONG_GUESSES_KEY2 860
#define G_KEY2 861
#define GUESSING_ARRAY_KEY2 862
#define GUESSED_LETTER_KEY2 863

//for word length 20
int create_shm(int);

int create_shm_int(int);

//for length 26
int create_shm_array(int);

void remove_shm(int);

char * get_shm(int);

char * get_shm_array(int);

int get_shm_int(int);

void set_shm(char *,int);

void set_shm_int(int,int);

void set_shm_array(char *,int);
