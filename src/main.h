#ifndef MAIN_H
#define MAIN_H
char * message;
char * buffer;

char * guessing_array;
char * guessed_letters;

int free_message = 0;
int free_g_buffer = 0;
int free_guessing_array = 0;
int free_guessed_letters = 0;

char ** list;

int main();

//free memory used
void free_list();

void free_mems();

// Clean up all sems and shm and free
// Basically whatever needs to be done when the server quits
void clean();
#endif
