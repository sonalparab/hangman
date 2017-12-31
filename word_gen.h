#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define DICTLOCATION "/usr/share/dict/american-english"
/* #define DICTLOCATION "testdict" */
#define MAXDICTLENGTH 100000
#define MAXWORDLENGTH 20

#ifndef WORDGEN_H
#define WORDGEN_H
char ** wordlist();
#endif
