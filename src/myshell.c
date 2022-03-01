#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include "myshell.h"


#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

extern char **environ;

void batchfileMode(int argc, char **argv);
void myshellMode(int argc, char **argv);


int main(int argc, char **argv) {

    
    if (argc == 2) { // Checks if args amount is 2 i.e ./myshell batchfile
        batchfileMode(argc, argv);
    } else { // Else goes into normal shell
        // TODO: Add I/O 
        myshellMode(argc, argv);
    }
    
    return 0;
}

void batchfileMode(int argc, char **argv) {

    char buf[MAX_BUFFER]; // line buffer

    char *currdir = getcwd(buf, MAX_BUFFER);
    char *shell = calloc(strlen(currdir) + 20, sizeof(char));
    strcat(shell, "SHELL=");
    strcat(shell, currdir); // Adds path to current directory
    strcat(shell, "/myshell");
    putenv(shell);


    FILE *file = fopen(argv[1], "r"); // Creates file pointer and opens it in read mode
    char line[MAX_BUFFER]; // creates a line with a max buffer

    if (file == NULL) { // Checks if file can be accessed
        fputs("myshell: Can not find or read file.\n", stdout);
    } else {
        while (fgets(line, sizeof(line), file) != NULL) {
            myprompt(buf);
            fputs(line, stdout);
            
            int array_size = 0;
            char ** cmd = inputstoargs(line, &array_size);

            int status; // Defines status for the fork

            my_commands(cmd, array_size, buf);
            
            wait(&status); // Waits for child process to finish
            fputs("\n", stdout);
        }
    }


    
}

void myshellMode(int argc, char **argv) {
    char buf[MAX_BUFFER]; // line buffer

    char *currdir = getcwd(buf, MAX_BUFFER);
    char *shell = calloc(strlen(currdir) + 20, sizeof(char));
    strcat(shell, "SHELL=");
    strcat(shell, currdir); // Adds path to current directory
    strcat(shell, "/myshell");
    putenv(shell);

    myprompt(buf);
    while (1) {

        if (fgets(buf, MAX_BUFFER, stdin)) {
            int array_size = 0;
            char ** cmd = inputstoargs(buf, &array_size);

            int status; // Defines status for the fork

            my_commands(cmd, array_size, buf);
            
            wait(&status); // Waits for child process to finish
            fputs("\n", stdout);
            myprompt(buf);
        }
    }
}