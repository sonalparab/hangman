#include "word_gen.h"

int main() {
    char **list = wordlist();
    if (list == NULL) {
        printf("Word generation failed...\n");
        exit(1);
    }
    int i = 0;

    // Writes the word list to a file
    FILE *f = fopen("generated", "w");
    for (i = 0; list[i]; i++) {
        int results = fputs(list[i], f);
        results = fputs("\n", f);
    }

    // Count the number of available words
    for (i = 0; list[i]; i++);
    int n = i;
    printf("len: %d\n", n);

    // Free all memory used
    for (i = 0; i < MAXDICTLENGTH; i++) {
        free(list[i]);
    }
    free(list);
    return 0;
}
