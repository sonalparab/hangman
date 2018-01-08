#include "word_gen.h"
#include "pipe_networking.h"
#include "game.h"
#include "main.h"

int main() {
    srand(time(NULL));

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
    char buffer[BUFFER_SIZE];
    /* while (read(from_client, buffer, sizeof(buffer))) { */
    while (1) {
        /* printf("[SERVER %d] received: %s\n", getpid(), buffer); */
        process(buffer, to_client, from_client);
    }
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
