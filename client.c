#include "pipe_networking.h"
#include "game.h"

int main() {

    int to_server;
    int from_server;
    char * buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    char input[100];

    from_server = client_handshake( &to_server );

    printf("[client] reading...");
    while (read(from_server, buffer, BUFFER_SIZE)) {
        if (strcmp(buffer, PROMPT) == 0) {
            printf(PROMPT);
	    fgets(input, sizeof(input), stdin);
	    *strchr(input, '\n') = 0;
            write(to_server, input, sizeof(input));
            printf("[client] wrote input: [%s]\n", input);
	    /*
            fgets(buffer, BUFFER_SIZE, stdin);
            *strchr(buffer, '\n') = 0;
            write(to_server, buffer, BUFFER_SIZE);
            printf("[client] wrote input: [%s]\n", buffer);
	    */

        } else {
            /* *strchr(buffer, '\n') = 0; */
            printf("received: [%s]\n", buffer);
            strcpy(buffer, ACK);
            write(to_server, buffer, BUFFER_SIZE);
            printf("[client] reading..\n");
        }
	input[0] = 0;
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }
}
