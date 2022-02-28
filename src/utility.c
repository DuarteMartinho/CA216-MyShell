#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

void myprompt(char *buf) {
    char * prompt = "$ " ;
    char *currdir = getcwd(buf, MAX_BUFFER);
    fputs(currdir, stdout);                  // shell prompt
    fputs("\n", stdout);                  // shell prompt
    fputs(prompt, stdout);                     // print prompt
}

