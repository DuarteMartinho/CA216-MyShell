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

/*
 * Function:  setInitialEnv
 * --------------------
 * Args: 
 *      1 - buf : line buffer
 * 
 * Purpose: 
 *      The purpose of this function is to set the inital environment variables
 * 
 */
void setInitialEnv(char *buf) {
    char *currdir = getcwd(buf, MAX_BUFFER); // Gets current directory
    char *shell = calloc(strlen(currdir) + 20, sizeof(char)); // Allocates memory to shell variable 
    strcat(shell, "SHELL="); // Adds SHELL to shell variable
    strcat(shell, currdir); // Adds current directory path to shell variable
    strcat(shell, "/myshell"); // Adds shell name to shell variable
    putenv(shell); // Puts shell variable into SHELL env
    char *shellpath = calloc(strlen(currdir) + 20, sizeof(char)); // Allocates memory to shellpath variable 
    strcat(shellpath, "SHELLPATH="); // Adds SSHELLPATH to shellpath variable 
    strcat(shellpath, currdir); // Adds current directory path to shellpath
    putenv(shellpath); // Puts shellpath variable into SHELLPATH env
}

/*
 * Function:  myprompt
 * --------------------
 * Args: 
 *      1 - buf : line buffer
 * 
 * Purpose: 
 *      The purpose of this function is to print out the prompt with current directory
 * 
 */
void myprompt(char *buf) {
    char * prompt = "$ " ; // Defines prompt
    char *currdir = getcwd(buf, MAX_BUFFER); // Gets current working directory
    fprintf(stdout, "%s\n%s", currdir, prompt); // Prints out to screen the prompt
}

/*
 * Function:  inputstoargs
 * --------------------
 * Args: 
 *      1 - input : line of input
 *      2 - size : pointer to size of input after split
 * 
 * Purpose: 
 *      The purpose of this function is to split the line of input and to count its number of args 
 *      Returns the line of input splitted and uses the pointer to return the size of the array
 * 
 */
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