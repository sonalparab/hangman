#include "subserver.h"
#include "word_gen.h"
#include "game.h"
#include "collab_game.h"
#include "competitive_game.h"
#include "sem.h"
#include "main.h"
#include "sharedmem.h"
#include "networking.h"

void subserver(int client_socket) {
    srand(time(NULL));
    char buffer[BUFFER_SIZE];

    char game_mode[10];

    char *prompt =(char *) calloc(BUFFER_SIZE,sizeof(char));
    strcpy(prompt,"Pick a game mode: ");
    write(client_socket,prompt,BUFFER_SIZE);
    printf("[subserver] Sent %s\n", prompt);

    prompt = zero_heap(prompt,BUFFER_SIZE);

    printf("[subserver] waiting for input\n");

    //prompt player for a mode
    int test = read(client_socket, game_mode, sizeof(game_mode));
    printf("[subserver] received input {%s}\n", game_mode);

    char mode = game_mode[0];

    //direct to the appropriate subserver
    if (mode == '1') {
        subserver_single(buffer, client_socket);
    } else if (mode == '2') {
        subserver_collab(buffer, client_socket);
    } else if (mode == '3') {
        subserver_competitive(buffer, client_socket);
    } else {
        strcpy(prompt,"Invalid mode: Single player");
        write(client_socket,prompt,BUFFER_SIZE);
        printf("[subserver] Sent %s\n", prompt);
        prompt = zero_heap(prompt, BUFFER_SIZE);
        test = read(client_socket, buffer, BUFFER_SIZE);
        subserver_single(buffer, client_socket);
    }

}

void subserver_single(char * buffer, int client_socket) {

    while (1) {
        process(buffer, client_socket);
    }

}

void subserver_collab(char * buffer, int client_socket) {

    //semaphore for multiple connects
    int semid = create_sem(KEY, 2);


    //if semid fails b/c already created
    if (semid == -1) {
        //get the semid
        semid = semget(KEY,1,0600);
    }

    decrement_sem(semid);
    int semval = view_sem(semid);


    while (1) {
        process_collab(buffer, client_socket);
    }
    increment_sem(semid);

}

void subserver_competitive(char * buffer, int client_socket) {

    //semaphore for multiple connects
    int semid = create_sem(KEY2,2);


    //if semid fails b/c already created
    if (semid == -1) {
        //get the semid
        semid = semget(KEY2,1,0600);
    }

    decrement_sem(semid);
    int semval = view_sem(semid);


    while (1) {
        process_competitive(buffer, client_socket);
    }
    increment_sem(semid);

}

void process(char * str, int client_socket) {
    // Pick 5 random words
    char *word;
    while (1) {
        word = word_pick(list);
        printf("Random word: %s\n", word);
        run_game(word, client_socket);
        free(word);
        printf("new len: %d\n", wordlist_len(list));
    }
    return;
}

void process_collab(char * str, int client_socket) {
    // Pick 5 random words
    char *word;
    while (1) {

        word = word_pick(list);
        //save word to shared memory if needed
        int shmid = create_shm(COLLABKEY);
        //shmid already created
        if (shmid == -1) {
            //get the shmid
            shmid = shmget(COLLABKEY, (sizeof(char) * 20),0600);
        } else {
            set_shm(word,shmid);
        }

        //get the word for the game
        char *sharedword = get_shm(shmid);

        run_game_collab(sharedword,client_socket);
        free(word);
        remove_shm(shmid);
        printf("new len: %d\n", wordlist_len(list));
    }
    return;
}

void process_competitive(char * str, int client_socket) {
    // Pick 5 random words
    char *word;
    while (1) {

        word = word_pick(list);

        //save word to shared memory if needed
        int shmid = create_shm(COMPETEKEY);
        //shmid already created
        if (shmid == -1) {
            //get the shmid
            shmid = shmget(COMPETEKEY, (sizeof(char) * 20),0600);
        } else {
            set_shm(word,shmid);
        }

        //get the word for the game
        char *sharedword = get_shm(shmid);

        run_game_competitive(sharedword,client_socket);
        free(word);
        remove_shm(shmid);
        printf("new len: %d\n", wordlist_len(list));
    }
    return;
}
