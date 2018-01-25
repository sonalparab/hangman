#include <setjmp.h>

#define SIGJMPENV 375

int game_running = 1;

char * buffer;

static void sighandler(int);

char * substring(char *, int);

int main();
