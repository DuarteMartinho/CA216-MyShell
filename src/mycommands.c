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

extern char **environ;

void do_cd(char **args, char *buf);
void do_clr();
void do_dir(char *dir);
void do_env();
void do_pause();
void do_echo(char **args, int array_size);
// void do_help();
void do_os(char **args);

void my_commands(char **cmd, int array_size, char *buf, int output, char *out_file) {

    if (cmd[0] != NULL && !strcmp(cmd[0],"quit")) {
        // quit command is in parent (outside fork) because cant exit from child process 
        _exit(0); 
        // exit(0) swapped to _exit(0) due to problem with fork when files where open
        // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
    } else if (cmd[0] != NULL && !strcmp(cmd[0],"dir")) {
        if (cmd[1] != NULL) {
            // dir command
            do_dir(cmd[1]);
        } else {
            fprintf(stdout, "myshell: Please provide a directory name or path.\n");
        }
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
                _exit(0); 
                // exit(0) swapped to _exit(0) due to problem with fork when files where open
                // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
            }
        }
    }
}

void do_cd(char **args, char *buf) {
    char *dir = args[1];
    if (args[1] == NULL) {
        // PWD
        char *currdir = getcwd(buf, MAX_BUFFER);
        fprintf(stdout, "%s\n", currdir);
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
        fprintf(stdout, "%s\n", *environ2++);
    }
}

void do_pause() {
    fputs("\nPress ENTER to continue ...\n", stdout);
    // https://stackoverflow.com/questions/28382962/wait-for-press-enter-in-c-inside-a-while-loop
    while(getchar()!='\n'); 
}

void do_echo(char **args, int array_size) {

    for (int i=1; i < array_size; i++) {
        fprintf(stdout, "%s ", args[i]);
    }
    fputs("\n", stdout);
}

void do_os(char **args) {
    int res = execvp(args[0], args);
    if (res == -1) {
        fprintf(stdout, "myshell: command not found\n");
    }
    _exit(0);
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}