#include "sem.h"

struct sembuf atomic_op;

int create_sem(int key, int val){
    semid = semget(key,1,IPC_CREAT | IPC_EXCL | 0600);
    if(semid == -1){
      //printf("could not create semaphore: %s\n",strerror(errno));
    }
    else{
        union semun su;
        su.val = val;
        int v = semctl(semid,0,SETVAL,su);
    }
    return semid;
}

void remove_sem(int semid){
    int v = semctl(semid,0,GETVAL);

    //block if semaphore val is 0
    while(v == 0)
        v = semctl(semid,0,GETVAL);

    //remove semaphore
    v = semctl(semid,0,IPC_RMID);
    if(v == -1)
      printf("could not remove semaphore: %s\n",strerror(errno));
    else
      printf("semaphore removed: %d \n",v);   
}


int view_sem(int semid){
    int v = semctl(semid,0,GETVAL);
    if(v == -1)
        printf("%s\n",strerror(errno));
    return v;
}

void decrement_sem(int semid){
    atomic_op.sem_op = -1; //down the semaphore
    atomic_op.sem_num = 0; //index of semaphore
    atomic_op.sem_flg = SEM_UNDO; //allow OS to undo given operation
    semop(semid, &atomic_op, 1);

}

void increment_sem(int semid){
    atomic_op.sem_op = 1; //up the semaphore
    atomic_op.sem_num = 0; //index of semaphore
    atomic_op.sem_flg = SEM_UNDO; //allow OS to undo given operation
    semop(semid, &atomic_op, 1);
}
