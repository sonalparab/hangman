#include "pipe_networking.h"
#include "game.h"

int main() {

    int to_server;
    int from_server;
    char buffer[BUFFER_SIZE];

    from_server = client_handshake( &to_server );

    while (read(from_server, buffer, sizeof(buffer))) {
        if (strcmp(buffer, PROMPT) == 0) {
            printf(PROMPT);
            fgets(buffer, sizeof(buffer), stdin);
            printf("[client] Input: [%s]\n", buffer);
        }
        /* *strchr(buffer, '\n') = 0; */
        printf("received: [%s]\n", buffer);
    }
}
