#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

// For use in actual game
#define DICTLOCATION "/usr/share/dict/american-english"
#define MAXDICTLENGTH 80000

// for use in testing
/* #define DICTLOCATION "testdict" */
/* #define MAXDICTLENGTH 10 */

#define MAXWORDLENGTH 20

#ifndef WORDGEN_H
#define WORDGEN_H

// Opens the dictionary location specified above, which on Ubuntu
// is a list of valid american english words
// Filter out words that we deem invalid for hangman, such as with
// less than 6 characters or has apostrophes
// Then generate a list of strings with the valid words
char ** wordlist();

// Given the list of words, take one at random and remove it from
// the list so it won't be chosen again
char * word_pick(char **);

// Given the list of words, return its length
int wordlist_len(char **);

// Lowers all characters in a string
unsigned char * lower(char *str);
#endif
