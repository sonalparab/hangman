#include "pipe_networking.h"
#include "game.h"

int main() {

    int to_server;
    int from_server;
    char buffer[BUFFER_SIZE];

    from_server = client_handshake( &to_server );

    printf("[client] reading...");
    while (read(from_server, buffer, sizeof(buffer))) {
        if (strcmp(buffer, PROMPT) == 0) {
            printf(PROMPT);
            fgets(buffer, sizeof(buffer), stdin);
            *strchr(buffer, '\n') = 0;
            printf("[client] Input: [%s]\n", buffer);
            write(to_server, buffer, sizeof(buffer));
        } else {
            /* *strchr(buffer, '\n') = 0; */
            printf("received: [%s]\n", buffer);
            buffer[0] = 0;
            printf("[client] reading..\n");
        }
    }
}
