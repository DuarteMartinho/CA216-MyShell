#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include "utility.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators
extern char **environ;

void do_cd(char **args, char *buf);
void do_clr();
void do_dir(char *dir);
void do_env();
void do_pause();
void do_echo(char **args, int array_size);
// void do_help();
void do_os(char **args);

char ** inputstoargs(char * input, int *size);

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
    fputs("BATCHFILE MODE\n", stdout);
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

            if (cmd[0] != NULL && !strcmp(cmd[0],"quit")) {
                // quit command is in parent (outside fork) because cant exit from child process 
                exit(0);
            } else if (cmd[0] != NULL && !strcmp(cmd[0],"dir") && cmd[1] != NULL) {
                // dir command
                do_dir(cmd[1]);
            } else if (cmd[0] != NULL && !strcmp(cmd[0],"environ")){
                // Get system environment variables
                do_env();
            } else if (cmd[0] != NULL && !strcmp(cmd[0],"cd")){
                // Change Directory
                do_cd(cmd, buf);
            } else if (cmd[0] != NULL && !strcmp(cmd[0],"echo")){
                if (cmd[1] != NULL) {
                    // Echo any arguments passed in to the command
                    do_echo(cmd, array_size);
                }
            } else {
                if (fork() == 0) {
                    if (!strcmp(cmd[0],"clr")) {
                        // Clear command
                        do_clr();
                    } else if (!strcmp(cmd[0],"pause")) {
                        // Pause command
                        do_pause();
                    } else if (cmd[0]) {
                        // Pass any other command to the os shell
                        do_os(cmd);
                    } else {
                        // Exits child process if no command given
                        exit(0);
                    }
                }
            }

            wait(&status); // Waits for child process to finish
            fputs("\n", stdout);
            myprompt(buf);
        }
    }
}




void do_cd(char **args, char *buf) {
    char *dir = args[1];
    if (args[1] == NULL) {
        // PWD
        char *currdir = getcwd(buf, MAX_BUFFER);
        fputs(currdir, stdout);
        fputs("\n", stdout);
    } else {
        int result = chdir(dir);
        if (result == 0) {
            char *currdir = getcwd(buf, MAX_BUFFER);
            char *pwd = calloc(strlen(currdir) + 5, sizeof(char));
            strcat(pwd, "PWD=");
            strcat(pwd, currdir);
            putenv(pwd);

        } else {
            fputs("myshell: Cannot cd to ", stdout);
            fputs(dir, stdout);
            fputs("\n", stdout);
        }
    }
}

void do_clr() {
    char *cmd[2] = { "clear", NULL};
    execvp("clear", cmd);
}

void do_dir(char *dir) {
    char *cmd = calloc(MAX_BUFFER, sizeof(char));
    strcat(cmd, "ls -la ");
    strcat(cmd, dir);
    system(cmd);
    free(cmd);
}

void do_env() {
    char **environ2 = environ;
    while (*environ2){
        fputs(*environ2++, stdout);
        fputs("\n", stdout);
    }
}

void do_pause() {
    fputs("\nPress ENTER to continue ...\n", stdout);
    // https://stackoverflow.com/questions/28382962/wait-for-press-enter-in-c-inside-a-while-loop
    while(getchar()!='\n'); 
}

void do_echo(char **args, int array_size) {

    for (int i=1; i < array_size; i++) {
        fputs(args[i], stdout);
        fputs(" ", stdout);
    }
    fputs("\n", stdout);
}

void do_os(char **args) {
    char *cmd = calloc(MAX_BUFFER, sizeof(char));
    while (*args) {
        strcat(cmd, *args++);
        strcat(cmd, " ");
    }
    system(cmd);
    free(cmd);
    exit(0);
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