#include "sharedmem.h"

int create_shm(int key){
    //create shared memory
    int shmid = shmget(key, (sizeof(char) * 20), IPC_CREAT | IPC_EXCL | 0600);
    if(shmid == -1)
      printf("could not create shared memory: %s\n",strerror(errno));
    else
      printf("shared memory created: %d\n",shmid);
    return shmid;
}

int create_shm_int(int key){
    //create shared memory
    int shmid = shmget(key, (sizeof(int)), IPC_CREAT | IPC_EXCL | 0600);
    if(shmid == -1)
      printf("could not create shared memory: %s\n",strerror(errno));
    else
      printf("shared memory created: %d\n",shmid);
    return shmid;
}


int create_shm_array(int key){
    //create shared memory
    int shmid = shmget(key, (sizeof(char) * 26), IPC_CREAT | IPC_EXCL | 0600);
    if(shmid == -1)
      printf("could not create shared memory: %s\n",strerror(errno));
    else
      printf("shared memory created: %d\n",shmid);
    return shmid;
}


void remove_shm(int shmid){
    //remove shared memory
    //shmid = shmget(key, sizeof(int), 0600);
    int s = shmctl(shmid,IPC_RMID,0);
    if(s == -1)
      printf("could not remove shared memory: %s\n",strerror(errno));
    else{
      printf("memory removed: %d\n", s);
    }
}

//hehe mem leak
char * get_shm(int shmid){
  //printf("shmid: %d\n",shmid);
  char *shmem = (char *)shmat(shmid,0,0);

  if(shmem == (char *)-1){
    printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
    return NULL;
  }
  else{
    printf("memory attached\n");
  }

  char *word = (char *)calloc(20,sizeof(char));

  strcpy(word,shmem);
  printf("word: %s\n",word);

  //free(word);

  int success = shmdt(shmem);
  if(success == -1)
    printf("failed for some reason: %s\n",strerror(errno));
  else
    printf("success detachement\n");

  return word;
 
}

char * get_shm_array(int shmid){
  //printf("shmid: %d\n",shmid);
  char *shmem = (char *)shmat(shmid,0,0);

  if(shmem == (char *)-1){
    printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
    return NULL;
  }
  else{
    printf("memory attached\n");
  }

  char *array = (char *)calloc(26,sizeof(char));

  strcpy(array,shmem);
  printf("word: %s\n",array);

  //free(word);

  int success = shmdt(shmem);
  if(success == -1)
    printf("failed for some reason: %s\n",strerror(errno));
  else
    printf("success detachement\n");

  return array;
 
}

int get_shm_int(int shmid){
  //printf("shmid: %d\n",shmid);
  int *shmem = (int *)shmat(shmid,0,0);

  if(shmem == (int *)-1){
    printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
    return -1;
  }
  else{
    printf("memory attached\n");
  }

  int i;

  i = *shmem;
  printf("int: %d\n",i);

  int success = shmdt(shmem);
  if(success == -1)
    printf("failed for some reason: %s\n",strerror(errno));
  else
    printf("success detachement\n");

  return i;
 
}

void set_shm(char *word,int shmid){
  printf("sharing word: %s\n",word);
  //printf("shmid: %d\n",shmid);
  
  char *shmem = (char *)shmat(shmid,0,0);

  if(shmem == (char *)-1){
    printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
    return;
  }
  else{
    printf("memory attached\n");
  }


  strcpy(shmem,word);
  

  int success = shmdt(shmem);
  if(success == -1)
    printf("failed for some reason: %s\n",strerror(errno));
  else
    printf("success detachment\n");

  
}

void set_shm_array(char* array,int shmid){
  printf("sharing word: %s\n",array);
  printf("shmid: %d\n",shmid);
  
  char *shmem = (char *)shmat(shmid,0,0);

  if(shmem == (char *)-1){
    printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
    return;
  }
  else{
    printf("memory attached\n");
  }


  strcpy(shmem,array);
  

  int success = shmdt(shmem);
  if(success == -1)
    printf("failed for some reason: %s\n",strerror(errno));
  else
    printf("success detachment\n");

  
}

void set_shm_int(int i,int shmid){
  printf("sharing int: %d\n",i);
  //printf("shmid: %d\n",shmid);
  
  int *shmem = (int *)shmat(shmid,0,0);

  if(shmem == (int *)-1){
    printf("Could not attach shared memory to pointer: %s\n",strerror(errno));
    return;
  }
  else{
    printf("memory attached\n");
  }


  //strcpy(shmem,word);

  *shmem = i;
  

  int success = shmdt(shmem);
  if(success == -1)
    printf("failed for some reason: %s\n",strerror(errno));
  else
    printf("success detachment\n");

}

/*
int main(){
  int id = create_shm(835);
  set_shm("hi there",id);
  printf("word: %s\n",get_shm(id));
  remove_shm(id);

  int id2 = create_shm_int(853);
  set_shm_int(7,id2);
  printf("int: %d\n",get_shm_int(id2));
  remove_shm(id2);
  
  int id3 = create_shm_array(856);
  char k[]= "asdfghjmkl";
  set_shm_array(k,id3);
  printf("array: %s\n",get_shm_array(id3));
  remove_shm(id3);


  char * guessed_letters = calloc(26,sizeof(char));

  guessed_letters[0] = 'y';
  guessed_letters[1] = 'k';
  guessed_letters[2] = 'g';
  guessed_letters[3] = 'p';
  guessed_letters[4] = 'i';
  
  
  int id4 = create_shm(860);
  set_shm(guessed_letters,id4);
  printf("array/word: %s\n",get_shm(id4));
  remove_shm(id4);

  free(guessed_letters);
  
}
*/
