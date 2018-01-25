#include "subserver.h"
#include "word_gen.h"
#include "game.h"
#include "collab_game.h"
#include "competitive_game.h"
#include "sem.h"
#include "sharedmem.h"
#include "main.h"
#include "networking.h"

static void sighandler(int signo){
    //to remove the semaphore for now
    if (signo == SIGINT) {
        remove("WKP");

        free_list();

        //remove semaphores used for player connection
        int semid = semget(KEY,1,0600);

        //if semid fails b/c doesn't exist
        if (semid == -1) {
            //printf("semaphore error: %s\n",strerror(errno));
        } else {
            remove_sem(semid);
        }

        semid = semget(KEY2,1,0600);

        //if semid fails b/c doesn't exist
        if (semid == -1) {
            //printf("semaphore error: %s\n",strerror(errno));
        } else {
            remove_sem(semid);
        }

        //remove sempahores used for turn taking in games
        semid = semget(COLLABKEY,1,0600);

        //if semid fails b/c doesn't exist
        if (semid == -1) {
            //printf("semaphore error: %s\n",strerror(errno));
        } else {
            remove_sem(semid);
        }

        semid = semget(COMPETEKEY,1,0600);

        //if semid fails b/c doesn't exist
        if (semid == -1) {
            //printf("semaphore error: %s\n",strerror(errno));
        } else {
            remove_sem(semid);
        }

        semid = semget(TURNKEY,1,0600);

        //if semid fails b/c doesn't exist
        if (semid == -1) {
            //printf("semaphore error: %s\n",strerror(errno));
        } else {
            remove_sem(semid);
        }

        //remove collab mode memory
        int shmid = shmget(COLLABKEY, (sizeof(char) * 20),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(GUESSING_ARRAY_KEY, (sizeof(char) * 20),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(GUESSED_LETTER_KEY, (sizeof(char) * 26),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(WRONG_GUESSES_KEY, (sizeof(int)),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(G_KEY, (sizeof(int)),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        //remove competitive mode memory
        shmid = shmget(COMPETEKEY, (sizeof(char) * 20),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(GUESSING_ARRAY_KEY2, (sizeof(char) * 20),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(GUESSED_LETTER_KEY2, (sizeof(char) * 26),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(WRONG_GUESSES_KEY2, (sizeof(int)),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
            remove_shm(shmid);
        }

        shmid = shmget(G_KEY2, (sizeof(int)),0600);

        //if shmid fails b/c doesn't exist
        if (shmid == -1) {
            //printf("sharedmem error: %s\n",strerror(errno));
        } else {
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

    // Writes the word list to a file
    int i = 0;
    FILE *file = fopen("generated", "w");
    for (i = 0; *(list[i]); i++) {
        int results = fputs(list[i], file);
        results = fputs("\n", file);
    }
    fclose(file);

    // Count the number of available words
    printf("len: %d\n", wordlist_len(list));
    int listen_socket;
    int f;
    listen_socket = server_setup();


    while (1) {
        int client_socket = server_connect(listen_socket);

        f = fork();
        if (f == 0) {
            subserver(client_socket);
            exit(0);
        } else {
            close(client_socket);
        }
    }

    free_list();
    return 0;
}

void free_list() {
    // Free all memory used
    int i = 0;
    for (i = 0; i < MAXDICTLENGTH; i++) {
        free(list[i]);
    }
    free(list);
}
