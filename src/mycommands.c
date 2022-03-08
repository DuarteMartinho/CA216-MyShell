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

extern char **environ;

// Function Prototypes
void do_cd(char **args, char *buf);
void do_clr();
void do_dir(char *dir);
void do_env();
void do_pause();
void do_echo(char **args, int array_size);
void do_help(char *buf);
void do_os(char **args);

void my_commands(char **cmd, int array_size, char *buf, int output, char *out_file) {

    if (cmd[0] != NULL && !strcmp(cmd[0],"quit")) { // Checks if cmd[0] is not null and then compares it to quit
        _exit(0); // quit command is in parent (outside fork) because cant exit from child process 
         
        // exit(0) swapped to _exit(0) due to problem with fork when files where open
        // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
    } else if (cmd[0] != NULL && !strcmp(cmd[0],"dir")) { // Checks if cmd[0] is not null and then compares it to dir
        if (cmd[1] != NULL) {
            do_dir(cmd[1]); // dir command function
        } else {
            fprintf(stdout, "myshell: Please provide a directory name or path.\n"); // Prints out to the screen some information to the user
        }
    } else if (cmd[0] != NULL && !strcmp(cmd[0],"environ")){ // Checks if cmd[0] is not null and then compares it to environ
        do_env(); // Get system environment variables function
    } else if (cmd[0] != NULL && !strcmp(cmd[0],"cd")){ // Checks if cmd[0] is not null and then compares it to cd
        // Change Directory function
        do_cd(cmd, buf);
    } else if (cmd[0] != NULL && !strcmp(cmd[0],"echo")){ // Checks if cmd[0] is not null and then compares it to echo
        if (cmd[1] != NULL) {
            // Echo any arguments passed in to the command function
            do_echo(cmd, array_size);
        }
    } else {
        if (fork() == 0) { // Creates a fork and checks if it was successful
            if (!strcmp(cmd[0],"clr")) { // Checks if cmd[0] is not null and then compares it to clr
                // Clear command function
                do_clr();
            } else if (!strcmp(cmd[0],"pause")) { // Checks if cmd[0] is not null and then compares it to pause
                // Pause command function
                do_pause();
            } else if (!strcmp(cmd[0],"help")) { // Checks if cmd[0] is not null and then compares it to help
                // Help command function
                do_help(buf);
            } else if (cmd[0]) { // Checks if cmd[0] is not null
                // Pass any other command to the os shell
                do_os(cmd);
            } else {
                _exit(0); // Exits child process if no command given
                // exit(0) swapped to _exit(0) due to problem with fork when files where open
                // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
            }
        }
    }
}

void do_cd(char **args, char *buf) {
    char *dir = args[1]; // Sets directory variable to args at position 1
    if (args[1] == NULL) {
        // PWD
        char *currdir = getcwd(buf, MAX_BUFFER); // Get current working directory
        fprintf(stdout, "%s\n", currdir); // Prints out to the screen some information to the user of print working directory
    } else {
        int result = chdir(dir); // Tries to change directory and stores result in a variable
        if (result == 0) { // if change directory is successful
            char *currdir = getcwd(buf, MAX_BUFFER); // Get current directory after directory change
            char *pwd = calloc(strlen(currdir) + 5, sizeof(char)); // Allocates memory to pwd variable
            strcat(pwd, "PWD="); // String concat PWD to variable
            strcat(pwd, currdir); // String concat current working directory to variable
            putenv(pwd); // Sets PWD env to current working directory
        } else {
            fprintf(stdout, "myshell: Cannot cd to %s\n", dir); // Prints out to the screen some information to the user
        }
    }
}

void do_clr() {
    char *cmd[2] = { "clear", NULL}; // Creates the command
    execvp("clear", cmd); // Executes the command
}

void do_dir(char *dir) {
    char *cmd = calloc(MAX_BUFFER, sizeof(char)); // Allocates memory to cmd
    strcat(cmd, "ls -la "); // String concat ls -la to the command 
    strcat(cmd, dir); // String concat desired directory to the command
    system(cmd); // Executes command
    free(cmd); // Frees up memory
}

void do_env() {
    char **environ2 = environ; // Sets point to environ
    while (*environ2){ // Loops through environ2
        fprintf(stdout, "%s\n", *environ2++); // Prints out to the screen the next environ line and increments it 
    }
}

void do_pause() {
    fputs("\nPress ENTER to continue ...\n", stdout); // Prints out to the screen some information to the user
    // https://stackoverflow.com/questions/28382962/wait-for-press-enter-in-c-inside-a-while-loop
    while(getchar()!='\n');  // While loop that waits for a new line (ENTER to be pressed)
}

void do_help(char *buf) {
    char * currdir = getcwd(buf, MAX_BUFFER); // Saves current directory to a variable
    chdir(getenv("SHELLPATH")); // Changes dir to shellpath  to be able to access the manual
    system("more ../manual/readme.md"); // Executes more command with the manual passed in
    chdir(currdir); // Changes dir to old current directory to keep user in same directory
}

void do_echo(char **args, int array_size) {

    for (int i=1; i < array_size; i++) { // Loops through until it reaches array size starting at 1 so that it skips `echo`
        fprintf(stdout, "%s ", args[i]); // Prints out the args at position i
    }
    fputs("\n", stdout); // Adds a new line at the end
}

void do_os(char **args) {
    int res = execvp(args[0], args); // Executes command and get a result in return
    if (res == -1) { // Checks if command exists using result value 
        fprintf(stdout, "myshell: command not found\n"); // Prints out to the screen some information to the user
    }
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}