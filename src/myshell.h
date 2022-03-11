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

// Function Prototypes
// utility.c
void setInitialEnv(char *buf);
char ** inputstoargs(char * input, int *size);
void myprompt(char *buf);

// mycommands.c
void my_commands(char **cmd, int array_size, char *buf);
