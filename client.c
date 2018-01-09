#include "pipe_networking.h"
#include "game.h"

int main() {

    int to_server;
    int from_server;
    char * buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));

    from_server = client_handshake( &to_server );

    printf("[client] reading...");
    while (read(from_server, buffer, sizeof(buffer))) {
        if (strcmp(buffer, PROMPT) == 0) {
            printf(PROMPT);
            fgets(buffer, sizeof(buffer), stdin);
            *strchr(buffer, '\n') = 0;
            write(to_server, buffer, sizeof(buffer));
            printf("[client] wrote input: [%s]\n", buffer);
        } else {
            /* *strchr(buffer, '\n') = 0; */
            printf("received: [%s]\n", buffer);
            strcpy(buffer, ACK);
            write(to_server, buffer, sizeof(buffer));
            printf("[client] reading..\n");
        }
        buffer[0] = 0;
    }
}
