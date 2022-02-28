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


char ** inputstoargs(char * input, int *size) {
    int i = 0; // Sets initial position
    char *arg; // Pointer for args
    char ** args = calloc(strlen(input), sizeof(char*)); // Creates args array

    arg = strtok(input, SEPARATORS); // Tokenizes the input i.e separates input
    while (arg != NULL) {
        args[i] = arg; // Setts current `arg` to the array `args` at position i
        i++; // Increments 1 to the position
        arg = strtok(NULL, SEPARATORS); // Advances to next arg
    }
    *size = i; // Sets size pointer to the last position of array
    return args; // Returns the array
}