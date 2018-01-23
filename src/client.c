/* #include "pipe_networking.h" */
#include "game.h"
#include "client.h"
#include "networking.h"

static void sighandler(int signo) {
    // Free buffer
    if (signo == SIGINT) {
        free(buffer);
    }

    exit(0);
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

    while (read(server_socket, buffer, BUFFER_SIZE)) {
        if (strcmp(buffer, MODE_PROMPT) == 0) {
            printf(MODE_PROMPT);
            fgets(mode, sizeof(mode), stdin);
            *strchr(mode, '\n') = 0;
            write(server_socket, mode, sizeof(mode));
            printf("[client] wrote mode: [%s]\n", mode);
        } else if (strcmp(buffer, GAME_PROMPT) == 0) {
            printf(GAME_PROMPT);
            fgets(input, sizeof(input), stdin);
            *strchr(input, '\n') = 0;
            write(server_socket, input, sizeof(input));
            printf("[client] wrote input: [%s]\n", input);
            /*
               fgets(buffer, BUFFER_SIZE, stdin);
             *strchr(buffer, '\n') = 0;
             write(server_socket, buffer, BUFFER_SIZE);
             printf("[client] wrote input: [%s]\n", buffer);
             */
        } else {
            /* *strchr(buffer, '\n') = 0; */
            printf("received: [%s]\n", buffer);
            strcpy(buffer, ACK);
            write(server_socket, buffer, BUFFER_SIZE);
        }
        input[0] = 0;
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }
}
