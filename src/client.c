#include "game.h"
#include "client.h"
#include "networking.h"

static void sighandler(int signo) {
    // Free buffer
    if (signo == SIGINT) {
        free(buffer);

        exit(0);
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

    buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    int server_socket;
    // If an argument, use the first as the ip
    if (argc == 2) {
        server_socket = client_setup( argv[1]);
    } else {
        server_socket = client_setup( TEST_IP );
    }

    char input[100];
    char mode[10];
    char *sub = 0;

    while (read(server_socket, buffer, BUFFER_SIZE)) {
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
        } else if (strcmp(sub = substring(buffer,3), "man") == 0){
            //for printing the man
            char * man = (char *) calloc(BUFFER_SIZE,sizeof(char));
            strncpy(man, buffer + 3, BUFFER_SIZE - 3);
            printf("%s\n", man);
            free(man);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        } else if (strcmp(sub = substring(buffer,8), "guessing") == 0){
            //for printing the guessing array
            char * guessing = (char *) calloc(BUFFER_SIZE,sizeof(char));
            strncpy(guessing, buffer + 8, BUFFER_SIZE - 8);
            printf("word: %s\n", guessing);
            free(guessing);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        } else if (strcmp(sub = substring(buffer,7), "guessed") == 0){
            //for printing the guessed letters
            char * guessed = (char *) calloc(BUFFER_SIZE,sizeof(char));
            strncpy(guessed, buffer + 7, BUFFER_SIZE - 7);
            printf("guessed letters: ");
            int i;
            for(i = 0; i < 26; i++){
                printf("%c ",guessed[i]);
            }
            printf("\n");
            free(guessed);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        } else {
            printf("%s\n", buffer);

            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        }
        // If sub was set (meaning something was calloc'd) free it
        if (sub) {
            free(sub);
            sub = 0;
        }
        input[0] = 0;
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }
}
