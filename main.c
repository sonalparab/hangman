#include "word_gen.h"
#include "pipe_networking.h"
#include "game.h"
#include "sem.h"
#include "main.h"
#include "sharedmem.h"

static void sighandler(int signo){
  //to remove the semaphore for now
  if(signo == SIGINT){
    
    int semid = semget(KEY,1,0600);
  
    //if semid fails b/c doesn't exist
    if(semid == -1){
      printf("semaphore error: %s\n",strerror(errno));
    }
    else{
      remove_sem(semid);
    }

    semid = semget(COLLABKEY,1,0600);
  
    //if semid fails b/c doesn't exist
    if(semid == -1){
      printf("semaphore error: %s\n",strerror(errno));
    }
    else{
      remove_sem(semid);
    }

    int shmid = shmget(COLLABKEY, (sizeof(char) * 20),0600);
    
    //shmid fails b/c doesn't exist
    if(shmid == -1){
      printf("sharedmem error: %s\n",strerror(errno));
    }
    else{
      remove_shm(shmid);
    }

    shmid = shmget(GUESSING_ARRAY_KEY, (sizeof(char) * 20),0600);
    
    //shmid fails b/c doesn't exist
    if(shmid == -1){
      printf("sharedmem error: %s\n",strerror(errno));
    }
    else{
      remove_shm(shmid);
    }

    shmid = shmget(GUESSED_LETTER_KEY, (sizeof(char) * 26),0600);
    
    //shmid fails b/c doesn't exist
    if(shmid == -1){
      printf("sharedmem error: %s\n",strerror(errno));
    }
    else{
      remove_shm(shmid);
    }

    shmid = shmget(WRONG_GUESSES_KEY, (sizeof(int)),0600);
    
    //shmid fails b/c doesn't exist
    if(shmid == -1){
      printf("sharedmem error: %s\n",strerror(errno));
    }
    else{
      remove_shm(shmid);
    }

    shmid = shmget(G_KEY, (sizeof(int)),0600);
    
    //shmid fails b/c doesn't exist
    if(shmid == -1){
      printf("sharedmem error: %s\n",strerror(errno));
    }
    else{
      remove_shm(shmid);
    }
    
    exit(0);
  }
}

int main() {

    signal(SIGINT, sighandler);
    
    list = wordlist();
    if (list == NULL) {
        printf("Word generation failed...\n");
        exit(1);
    }
    int i = 0;

    // Writes the word list to a file
    FILE *f = fopen("generated", "w");
    for (i = 0; *(list[i]); i++) {
        int results = fputs(list[i], f);
        results = fputs("\n", f);
    }
    fclose(f);

    // Count the number of available words
    printf("len: %d\n", wordlist_len(list));

    int from_client;
    char buffer[HANDSHAKE_BUFFER_SIZE];
    while (1) {
        from_client = server_setup(buffer);

        int f = fork();
        if (f == 0) {
	    subserver(from_client);
            exit(0);
        }
    }

    // Free all memory used
    for (i = 0; i < MAXDICTLENGTH; i++) {
        free(list[i]);
    }
    free(list);
    return 0;
}



void subserver(int from_client) {
    int to_client = server_connect(from_client);
    srand(time(NULL));
    char buffer[BUFFER_SIZE];
    /* while (read(from_client, buffer, sizeof(buffer))) { */

    char game_mode[10];

    char *choice =(char *) calloc(BUFFER_SIZE,sizeof(char));
    strcpy(choice,"Pick a game mode: ");
    write(to_client,choice,BUFFER_SIZE);
    printf("[subserver] Sent %s\n", choice);

    choice = zero_heap(choice,BUFFER_SIZE);
    
    printf("[subserver] waiting for input\n");
    //prompt input for a letter
    int test = read(from_client, game_mode, sizeof(game_mode));
    printf("[subserver] received input {%s}\n", game_mode);

    char mode = game_mode[0];
 	    
    if(mode == '1'){
      subserver_single(buffer,to_client,from_client);
    }
    else if(mode == '2'){
      subserver_collab(buffer, to_client,from_client);
    }
    else{
      printf("Smh stop tryna break the code: single player");
      subserver_single(buffer,to_client,from_client);
    }

    /*  
    while (1) {
        process(buffer, to_client, from_client);
    }
    */
}

void subserver_single(char * buffer, int to_client, int from_client) {
     
    while (1) {
      /* printf("[SERVER %d] received: %s\n", getpid(), buffer); */
      process(buffer, to_client, from_client);
    }

}

void subserver_collab(char * buffer, int to_client, int from_client) {
  
  //sem stuff for multiple connects
  int semid = create_sem(KEY,2);
  

  //if semid fails b/c already created
  if(semid == -1){
    printf("semaphore error: %s\n",strerror(errno));
    //get the semid
    semid = semget(KEY,1,0600);
  }

  decrement_sem(semid);
  int semval = view_sem(semid);

    
  while (1) {
    /* printf("[SERVER %d] received: %s\n", getpid(), buffer); */
    process_collab(buffer, to_client, from_client);
  }
  increment_sem(semid);

}

void process(char * str, int to_client, int from_client) {
    // Pick 5 random words
    char *word;
    while (1) {
        word = word_pick(list);
        printf("Random word: %s\n", word);
        //trying the game
        run_game(word, to_client, from_client);
        free(word);
        printf("new len: %d\n", wordlist_len(list));
    }
    return;
}

void process_collab(char * str, int to_client, int from_client) {
    // Pick 5 random words
    char *word;
    while (1) {

        word = word_pick(list);
	printf("Random word: %s\n", word);

	//save word to shared memory if needed
        int shmid = create_shm(COLLABKEY);
	//shmid already made
	if(shmid == -1){
	  shmid = shmget(COLLABKEY, (sizeof(char) * 20),0600);
	}
	else{
	  set_shm(word,shmid);
	}

	printf("SHMID: %d  ",shmid);
	char *sharedword = get_shm(shmid);
	printf("Shared word is: %s  ",sharedword);
	
	run_game_collab(sharedword,to_client,from_client);;
        free(word);
	remove_shm(shmid);
        printf("new len: %d\n", wordlist_len(list));
    }
    return;
}
