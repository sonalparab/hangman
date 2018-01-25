#include "game.h"
#include "client.h"
#include "networking.h"

sigjmp_buf env;

static void sighandler(int signo) {
    // Free buffer
    if (signo == SIGINT) {
        if (game_running) {
            printf("Please complete your current game before quitting.\n");
            siglongjmp(env, SIGJMPENV);
        } else {
            printf("Quitting...\n");
            free(buffer);
            exit(0);
        }
    }
}

char * substring(char * str, int n){
    char * ans = (char *) calloc(n + 1,sizeof(char));
    int i;
    for(i = 0; i < n; i++){
        ans[i] = str[i];
    }
    ans[i] = 0;
    return ans;
}

int main(int argc, char **argv) {

    signal(SIGINT, sighandler);

    int server_socket;
    // If an argument, use the first as the ip
    if (argc == 2) {
        server_socket = client_setup( argv[1]);
    } else {
        server_socket = client_setup( TEST_IP );
    }

    buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    char input[100];
    char mode[10];
    char play_again[100];

    while (read(server_socket, buffer, BUFFER_SIZE)) {
        sigsetjmp(env, 1) == SIGJMPENV;
        if (strcmp(buffer, MODE_PROMPT) == 0) {
            printf("Game modes:\n");
            printf("Pick 1 for single player\n");
            printf("Pick 2 for collab mode\n");
            printf("Pick 3 for competitive mode\n");
            printf(MODE_PROMPT);
            fgets(mode, sizeof(mode), stdin);
            *strchr(mode, '\n') = 0;
            write(server_socket, mode, sizeof(mode));
            //printf("[client] wrote mode: [%s]\n", mode);
        } else if (strcmp(buffer, GAME_PROMPT) == 0) {
            printf(GAME_PROMPT);
            fgets(input, sizeof(input), stdin);
            *strchr(input, '\n') = 0;
            write(server_socket, input, sizeof(input));
        } else if (strcmp(substring(buffer,3), "man") == 0){
            //for printing the man
            char * man = (char *) calloc(BUFFER_SIZE,sizeof(char));
            strncpy(man, buffer + 3, BUFFER_SIZE - 3);
            printf("%s\n", man);
            man = zero_heap(man, BUFFER_SIZE);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        } else if (strcmp(substring(buffer,8), "guessing") == 0){
            //for printing the guessing array
            char * guessing = (char *) calloc(BUFFER_SIZE,sizeof(char));
            strncpy(guessing, buffer + 8, BUFFER_SIZE - 8);
            printf("word: %s\n", guessing);
            guessing = zero_heap(guessing, BUFFER_SIZE);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        } else if (strcmp(substring(buffer,7), "guessed") == 0){
            //for printing the guessed letters
            char * guessed = (char *) calloc(BUFFER_SIZE,sizeof(char));
            strncpy(guessed, buffer + 7, BUFFER_SIZE - 7);
            printf("guessed letters: ");
            int i;
            for(i = 0; i < 26; i++){
                printf("%c ",guessed[i]);
            }
            printf("\n");
            guessed = zero_heap(guessed, BUFFER_SIZE);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        } else if (strcmp(buffer, WIN_MSG) == 0 || strcmp(buffer, LOSE_MSG) == 0) {
            game_running = 0;
            int valid = 0;
            char entry;
            while (!valid) {
                printf("Would you like to play again? (y/n) ");
                fgets(play_again, sizeof(play_again), stdin);
                *strchr(play_again, '\n') = 0;
                if (strlen(play_again) > 1) {
                    printf("Please enter y or n!\n");
                    continue;
                }
                entry = play_again[0];
                if (entry != 'y' && entry != 'n') {
                    printf("%c %d\n", entry);
                    printf("Please enter y or n.\n");
                    continue;
                }
                valid = 1;
            }
            if (entry == 'n') {
                printf("Quitting...\n");
                exit(0);
            } else {
                strcpy(buffer, ACK);
                write(server_socket, buffer, BUFFER_SIZE);
            }
        } else {
            printf("%s\n", buffer);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        }
        input[0] = 0;
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }
}
