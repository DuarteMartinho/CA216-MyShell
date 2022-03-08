// Name: Duarte Martinho
// Student Number: 20410974
// I acknowledge DCU's Academic Integrity Policy

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token separators

void myprompt(char *buf) {
    char * prompt = "$ " ; // Defines prompt
    char *currdir = getcwd(buf, MAX_BUFFER); // Gets current working directory
    fprintf(stdout, "%s\n%s", currdir, prompt); // Prints out to screen the prompt
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