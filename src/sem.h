#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

#define KEY 832
#define KEY2 833
#define COLLABKEY 835
#define COMPETEKEY 837
#define TURNKEY 839

int semid;
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array; 
    struct seminfo *__buf;
};


#ifndef SEM
#define SEM
//create a semaphore with a given key, set to a
// given value
int create_sem(int,int);

//remove a semaphore, given the id
void remove_sem(int);

//get the value of the semaphore, given the id
int view_sem(int);

//decrement the semaphore, given the id
void decrement_sem(int);

//increment the semaphore, given the id
void increment_sem(int);
#endif
