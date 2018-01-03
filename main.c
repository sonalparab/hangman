#include "word_gen.h"
#include "game.h"

int main() {
    srand(time(NULL));

    char **list = wordlist();
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

    // Pick 5 random words
    char *word;
    for (i = 0; i < 5; i++) {
        word = word_pick(list);
        printf("Random word: %s\n", word);
	//trying the game
	run_game(word);
        free(word);
        printf("new len: %d\n", wordlist_len(list));
    }

    // Free all memory used
    for (i = 0; i < MAXDICTLENGTH; i++) {
        free(list[i]);
    }
    free(list);
    return 0;
}
