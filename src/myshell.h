#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

// utility.c
void myprompt(char *buf);
char ** inputstoargs(char * input, int *size);

// mycommands.c
void my_commands(char **cmd, int array_size, char *buf);
