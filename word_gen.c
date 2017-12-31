#include "word_gen.h"

// Generates the list of words to be used in the hangman game
char ** wordlist() {
    // Open the original dict
    FILE *f = fopen(DICTLOCATION, "r");
    // If dict cannot be found, return with error
    if (f == NULL) {
        return 1;
    }

    // Allocate memory for all the words
    char **wordlist = (char **) malloc(sizeof(char *) * MAXDICTLENGTH);
    int i = 0;
    for (i = 0; i < MAXDICTLENGTH; i++) {
        wordlist[i] = (char *) malloc(sizeof(char) * MAXWORDLENGTH);
    }

    // Read each word
    // Reject the word if len < 6 or has apostrophes
    i = -1;
    while (++i < MAXDICTLENGTH && fgets(wordlist[i], MAXWORDLENGTH * sizeof(char), f)) {
        char *newline = strchr(wordlist[i], '\n');
        if (newline) {
            *newline = 0;
        }
        if (strlen(wordlist[i]) < 6) {
            /* printf("word rejected: %s\n", wordlist[i]); */
            i--;
        } else {
            char *apostrophe = strchr(wordlist[i], '\'');
            if (apostrophe) {
                i--;
            }
        }
    }

    // Make sure the list is null-terminated (for ease of use)
    wordlist[i] = 0;
    printf("[INIT] Word list generated.\n");
    return wordlist;
}
