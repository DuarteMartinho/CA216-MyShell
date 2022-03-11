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
#include "myshell.h"


#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token separators

extern char **environ;

// Function Prototypes
void batchfileMode(int argc, char **argv);
void myshellMode(int argc, char **argv);

/*
 * Function:  main
 * --------------------
 * Args: 
 *      1 - argc : number of arguments
 *      2 - argv : command line arguments
 * 
 * Purpose: 
 *      The purpose of this function is main function of the program
 *      In this function we decide if we want normal mode or batch mode
 * 
 * 
 */
int main(int argc, char **argv) {
    if (argc == 2) { // Checks if args amount is 2 i.e ./myshell batchfile
        batchfileMode(argc, argv); // Calls batchfileMode function passing in argv and argc
    } else { // Else goes into normal shell 
        myshellMode(argc, argv); // Calls myshellMode function
    }
    return 0;
}

/*
 * Function:  batchfileMode
 * --------------------
 * Args: 
 *      1 - argc : number of arguments
 *      2 - command line arguments
 * 
 * Purpose: 
 *      The purpose of this function is to run myshell in batch mode
 *      This means that the commands will be read from a file line by line and will then be executed and printed out the results
 * 
 */
void batchfileMode(int argc, char **argv) {

    char buf[MAX_BUFFER]; // line buffer
    setInitialEnv(buf); // Calls setInitialEnv function to set the initial environment variables

    FILE *file = fopen(argv[1], "r"); // Creates file pointer and opens it in read mode
    char line[MAX_BUFFER]; // creates a line with a max buffer

    if (file == NULL) { // Checks if file can be accessed
        fputs("myshell: Can not find or read file.\n", stdout); // Prints error message to the screen
    } else {
        while (fgets(line, sizeof(line), file) != NULL) { // Loops until the end of the file is reached
            myprompt(buf); // Calls myprompt function to print out the function
            fputs(line, stdout); // Prints out the cmd inputed
            
            int array_size = 0; // Initializes array_size 
            char ** cmd = inputstoargs(line, &array_size); // Calls inputstoargs function to tokenize the line and to get the number of args
            my_commands(cmd, array_size, buf); // Calls my_commands function passing in the command splitted in args
            fputs("\n", stdout); // Prints out a new line
        }
    } 
}

/*
 * Function:  myshellMode
 * --------------------
 * Args: 
 *      1 - argc : number of arguments
 *      2 - command line arguments
 * 
 * Purpose: 
 *      The purpose of this function is to execute the shell in normal mode
 *      This means that it asks the user for a prompt each time 
 * 
 */
void myshellMode(int argc, char **argv) {
    char buf[MAX_BUFFER]; // line buffer
    setInitialEnv(buf); // Calls setInitialEnv function to set the initial environment variables
    myprompt(buf); // Calls myprompt function to print out the function
    while (1) { // Infinite loop until quit command or stopped 
        if (fgets(buf, MAX_BUFFER, stdin)) { // Gets the next input line from stdin 
            int array_size = 0; // Initializes array_size 
            char ** cmd = inputstoargs(buf, &array_size); // Calls inputstoargs function to tokenize the line and to get the number of args
            my_commands(cmd, array_size, buf); // Calls my_commands function passing in the command splitted in args
            fputs("\n", stdout); // Prints out a new line
            myprompt(buf); // Calls myprompt function to print out the function
        }
    }
}