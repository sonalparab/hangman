#include "sharedmem.h"

//functions for char * size 20
int create_shm(int key){
    int shmid = shmget(key, (sizeof(char) * 20), IPC_CREAT | IPC_EXCL | 0600);
    /*if(shmid == -1)
        printf("could not create shared memory: %s\n",strerror(errno));
    else
        printf("shared memory created: %d\n",shmid);*/
    return shmid;
}

char * get_shm(int shmid){
    char *shmem = (char *)shmat(shmid,0,0);

    if(shmem == (char *)-1){
        printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
        return NULL;
    }
    
    char *word = (char *)calloc(20,sizeof(char));
    strcpy(word,shmem);

    int success = shmdt(shmem);
    if(success == -1)
        printf("failed for some reason: %s\n",strerror(errno));
  
    return word;

}

void set_shm(char *word,int shmid){
    char *shmem = (char *)shmat(shmid,0,0);

    if(shmem == (char *)-1){
        printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
        return;
    }

    strcpy(shmem,word);

    int success = shmdt(shmem);
    if(success == -1)
        printf("failed for some reason: %s\n",strerror(errno));

}

//functions for int
int create_shm_int(int key){
    //create shared memory for ints
    int shmid = shmget(key, (sizeof(int)), IPC_CREAT | IPC_EXCL | 0600);
    /*if(shmid == -1)
        printf("could not create shared memory: %s\n",strerror(errno));
    else
        printf("shared memory created: %d\n",shmid);*/
    return shmid;
}

int get_shm_int(int shmid){
    int *shmem = (int *)shmat(shmid,0,0);

    if(shmem == (int *)-1){
        printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
        return -1;
    }

    int i;
    i = *shmem;

    int success = shmdt(shmem);
    if(success == -1)
        printf("failed for some reason: %s\n",strerror(errno));

    return i;

}

void set_shm_int(int i,int shmid){
    int *shmem = (int *)shmat(shmid,0,0);

    if(shmem == (int *)-1){
        printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
        return;
    }

    *shmem = i;


    int success = shmdt(shmem);
    if(success == -1)
        printf("failed for some reason: %s\n",strerror(errno));
   
}

//functions for char * size 26
int create_shm_array(int key){
    int shmid = shmget(key, (sizeof(char) * 26), IPC_CREAT | IPC_EXCL | 0600);
    /*if(shmid == -1)
        printf("could not create shared memory: %s\n",strerror(errno));
    else
        printf("shared memory created: %d\n",shmid);*/
    return shmid;
}

char * get_shm_array(int shmid){
    char *shmem = (char *)shmat(shmid,0,0);

    if(shmem == (char *)-1){
        printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
        return NULL;
    }
   
    char *array = (char *)calloc(26,sizeof(char));

    strcpy(array,shmem);

    int success = shmdt(shmem);
    if(success == -1)
        printf("failed for some reason: %s\n",strerror(errno));

    return array;

}

void set_shm_array(char* array,int shmid){
    char *shmem = (char *)shmat(shmid,0,0);

    if(shmem == (char *)-1){
        printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
        return;
    }

    strcpy(shmem,array);

    int success = shmdt(shmem);
    if(success == -1)
        printf("failed for some reason: %s\n",strerror(errno));

}

//remove the shared memory
void remove_shm(int shmid){
    int s = shmctl(shmid,IPC_RMID,0);
    if(s == -1){
        //printf("could not remove shared memory: %s\n",strerror(errno));
    }
    else{
        printf("memory removed: %d\n", s);
    }
}
