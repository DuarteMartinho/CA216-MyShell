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
#include <fcntl.h>

#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token separators

extern char **environ;

// Function Prototypes
void do_cd(char **args, char *buf);
void do_clr();
void do_dir(char *dir, char * file);
void do_env(char * file);
void do_pause();
void do_echo(char **args, int array_size, char * file);
void do_help(char *buf, char * file);
void do_os(char **args, char * file);
void ioredirection(char * file);

/*
 * Function:  my_commands
 * --------------------
 * Args: 
 *      1 - cmd : command splitted into args
 *      2 - array_size : number of arguments in command
 *      3 - buf : line buffer
 * 
 * Purpose:
 *      This function is used to loop through the command arguments and to check for I/O redirection. And after check what command
 *      it is, either internal or external.
 * 
 */
void my_commands(char **cmd, int array_size, char *buf) {
    char *out_file = NULL; // Sets output file variable name to stdout by default
    char *customCmd[MAX_ARGS]; // Custom arguments array
    int customCmd_count = 0; // Number of custom arguments
    int backExec = 0; // defines background exec by default to false

    for (int i = 0; i < array_size; i++) { // Goes through the cmd array checking for I/O redirection
        if (!strcmp(cmd[i],">")) { // Checks if cmd[i] is >

            if (cmd[i + 1] != NULL) { // Checks if cmd[i + 1] is not NULL
                i++; // Increment i so that it moves to the file name instead of '>'
                fopen(cmd[i], "w+"); // Opens file and truncates it
                out_file = calloc(strlen(cmd[i]) + 2, sizeof(char)); // Allocates memory to the out file so that it can save the file name in the variable
                strcpy(out_file, cmd[i]); // Save outfile name
            } else {
                fputs("myshell: No file provided", stdout); // Prints out the error message to display
            }

        } else if (!strcmp(cmd[i],">>")) { // Checks if cmd[i] is >>

            if (cmd[i + 1] != NULL) { // Checks if cmd[i + 1] is not NULL
                i++; // Increment i so that it moves to the file name instead of '>>'
                out_file = calloc(strlen(cmd[i]) + 2, sizeof(char)); // Allocates memory to the out file so that it can save the file name in the variable
                strcpy(out_file, cmd[i]); // Save outfile name
            } else {
                fputs("myshell: No file provided", stdout); // Prints out the error message to display
            }

        } else if (!strcmp(cmd[i],"<")) { // Checks if cmd[i] is <

            if (cmd[i + 1] != NULL) { // Checks if cmd[i + 1] is not NULL
                i++; // Increment i so that it moves to the file name instead of '<'
                customCmd[customCmd_count] = "<"; // Add input char to the array
                customCmd_count++; // Increment customCmd_count so that it moves to the file name
                customCmd[customCmd_count] = cmd[i]; // Adds cmd[i] (FILE NAME ) to the array
                customCmd_count++; // Increment customCmd_count so that it moves to the next arg
            } else {
                fputs("myshell: No file provided", stdout); // Prints out the error message to display
            }

        } else if (!strcmp(cmd[i],"&")) { // Checks if cmd[i] is &

            backExec = 1; // Sets background exec to be true

        } else { // Else just adds the other args to the array

            customCmd[customCmd_count] = cmd[i]; // Saves customs args
            customCmd_count++; // Increments custom args count by 1
        
        }
    }
    customCmd[customCmd_count] = NULL;

    if (customCmd[0] != NULL && !strcmp(customCmd[0],"quit")) { // Checks if cmd[0] is not null and then compares it to quit
        _exit(0); // quit command is in parent (outside fork) because cant exit from child process 
        // exit(0) swapped to _exit(0) due to problem with fork when files where open
        // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
    } else {

        pid_t myfork = fork(); // Creates a fork
        if (myfork == 0) { // Checks if the fork was successful
            if (backExec == 1) { // Check if background exec is being used
                fprintf(stdout, "\n"); // Prints Out new line to stdout
            }
            if (!strcmp(customCmd[0],"clr")) { // Checks if customCmd[0] is not null and then compares it to clr
                // Clear command function
                do_clr();
            } else if (!strcmp(customCmd[0],"pause")) { // Checks if customCmd[0] is not null and then compares it to pause
                // Pause command function
                do_pause();
            } else if (!strcmp(customCmd[0],"help")) { // Checks if customCmd[0] is not null and then compares it to help
                // Help command function
                do_help(buf, out_file);
            } else if (customCmd[0] != NULL && !strcmp(customCmd[0],"dir")) { // Checks if cmd[0] is not null and then compares it to dir
                if (customCmd[1] != NULL) {
                    do_dir(customCmd[1], out_file); // dir command function
                } else {
                    fprintf(stdout, "myshell: Please provide a directory name or path.\n"); // Prints out to the screen some information to the user
                }
            } else if (customCmd[0] != NULL && !strcmp(customCmd[0],"environ")){ // Checks if cmd[0] is not null and then compares it to environ
                do_env(out_file); // Get system environment variables function
            } else if (customCmd[0] != NULL && !strcmp(customCmd[0],"cd")){ // Checks if cmd[0] is not null and then compares it to cd
                // Change Directory function
                do_cd(customCmd, buf);
            } else if (customCmd[0] != NULL && !strcmp(customCmd[0],"echo")){ // Checks if customCmd[0] is not null and then compares it to echo
                if (customCmd[1] != NULL) {
                    // Echo any arguments passed in to the command function
                    do_echo(customCmd, array_size, out_file);
                }
            } else if (customCmd[0] != NULL) { // Checks if customCmd[0] is not null
                // Pass any other command to the os shell
                do_os(customCmd, out_file);
            } else {
                _exit(0); // Exits child process if no command given
                // exit(0) swapped to _exit(0) due to problem with fork when files where open
                // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
            }
        }
        if (backExec == 1) { // Check if background exec is being used
            fprintf(stdout, "\n[PID] %d\n", myfork); // Prints out PID information to the user 
        } else {
            waitpid(myfork, &myfork, 0); // Waits for child process to finish
        }
        
        if (backExec == 0) {  // Check if background exec is not being used
        }
    }
}

/*
 * Function:  do_cd
 * --------------------
 * Args: 
 *      1 - args : command args
 *      2 - buf : line buffer
 * 
 * Purpose:
 *      The purpose of this function is to change directory to a given path in `args`
 *      It uses chdir and putenv to change directory and update environment variable
 * 
 */
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

/*
 * Function:  do_clr
 * --------------------
 * Args: 
 *      This functions takes no args
 * 
 * Purpose:
 *      The purpose of this function is to clear the sceeen
 *      It uses execvp to execute the command
 * 
 */
void do_clr() {
    char *cmd[2] = { "clear", NULL}; // Creates the command
    execvp(cmd[0], cmd); // Executes the command
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  do_dir
 * --------------------
 * Args: 
 *      1 - dir : name of directory or the path to the directory
 *      2 - file : NULL or name of file for the output
 * 
 * Purpose:
 *      The purpose of this function is to list all files and directories in the given directory
 *      It uses execvp to execute the commande
 * 
 */
void do_dir(char *dir, char * file) {
    if (file != NULL) { // Checks if there is a input file set
        ioredirection(file); // Calls I/O redirection where it will swap the stdout for the file
    }
    char *cmd[4] = { "ls", "-la", dir, NULL}; // Creates the command
    execvp(cmd[0], cmd); // Executes command
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  do_env
 * --------------------
 * Args:
 *      1 - file : NULL or name of file for the output 
 * 
 * Purpose:
 *      The purpose of this function is to print out all the environment variables
 *      It uses environ and it creates a pointer for environ and then it goes through the pointer
 * 
 */
void do_env(char * file) {
    if (file != NULL) { // Checks if there is a input file set
        ioredirection(file); // Calls I/O redirection where it will swap the stdout for the file
    }
    char **environ2 = environ; // Sets point to environ
    while (*environ2){ // Loops through environ2
        fprintf(stdout, "%s\n", *environ2++); // Prints out to the screen the next environ line and increments it 
    }
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  do_pause
 * --------------------
 * Args: 
 *      This functions takes no args
 * 
 * Purpose:
 *      The purpose of this function is to pause the prompt until the user presses ENTER
 *      It has a while until it gets a new line
 * 
 */
void do_pause() {
    fputs("\nPress ENTER to continue ...\n", stdout); // Prints out to the screen some information to the user
    // https://stackoverflow.com/questions/28382962/wait-for-press-enter-in-c-inside-a-while-loop
    while(getchar()!='\n');  // While loop that waits for a new line (ENTER to be pressed)
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  do_help
 * --------------------
 * Args: 
 *      1 - buf : line buffer
 *      2 - file : NULL or name of file for the output 
 * 
 * Purpose:
 *      The purpose of this function is to print out the help manual
 *      It changes directory to the manual folder then uses execvp to execute the command 
 *      and then it changes directory back to the original directory
 * 
*/
void do_help(char *buf, char * file) {
    if (file != NULL) { // Checks if there is a input file set
        ioredirection(file); // Calls I/O redirection where it will swap the stdout for the file
    }
    char * currdir = getcwd(buf, MAX_BUFFER); // Saves current directory to a variable
    chdir(getenv("SHELLPATH")); // Changes dir to shellpath  to be able to access the manual
    chdir("../manual/"); // Changes dir to manual folder
    char *cmd[3] = { "more", "readme.md", NULL}; // Creates the command
    execvp(cmd[0], cmd); // Executes more command with the manual file passed in
    chdir(currdir); // Changes dir to old current directory to keep user in same directory
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  do_echo
 * --------------------
 * Args: 
 *      1 - args : line buffer
 *      2 - array_size : number of arguments in command
 *      3 - file : NULL or name of file for the output 
 * 
 * Purpose:
 *      The purpose of this function is to print out the args given
 *      It prints out the arguments to the stdout or to the I/O redirection
 * 
 */
void do_echo(char **args, int array_size, char * file) {
    if (file != NULL) { // Checks if there is a input file set
        ioredirection(file); // Calls I/O redirection where it will swap the stdout for the file
    }
    for (int i=1; i < array_size; i++) { // Loops through until it reaches array size starting at 1 so that it skips `echo`
        if (args[i] == NULL) { // Checks if the arg is not NULL
            break;
        }
        fprintf(stdout, "%s " , args[i]); // Prints out the args at position i
    }
    fprintf(stdout, "\n"); // Adds a new line at the end
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  do_os
 * --------------------
 * Args: 
 *      1 - args : line buffer
 *      2 - file : NULL or name of file for the output 
 * 
 * Purpose:
 *      The purpose of this function is to execute any other external command
 *      It uses execvp to execute the command and it prints it to stdout or I/O redirection
 * 
 */
void do_os(char **args, char * file) {
    if (file != NULL) { // Checks if there is a input file set
        ioredirection(file); // Calls I/O redirection where it will swap the stdout for the file
    }
    int res = execvp(args[0], args); // Executes command and get a result in return
    if (res == -1) { // Checks if command exists using result value 
        fprintf(stdout, "myshell: command not found\n"); // Prints out to the screen some information to the user
    }
    _exit(0); // Exits the fork()
    // exit(0) swapped to _exit(0) due to problem with fork when files where open
    // https://stackoverflow.com/questions/50110992/why-does-forking-my-process-cause-the-file-to-be-read-infinitely
}

/*
 * Function:  ioredirection
 * --------------------
 * Args: 
 *      1 - file : file name
 * 
 * Purpose:
 *      The purpose is to open a file and redirect it to stdout.
 */
void ioredirection(char * file) {
    // https://stackoverflow.com/questions/10705612/how-to-open-a-file-in-append-mode-using-open-fopen
    // https://man7.org/linux/man-pages/man2/open.2.html
    int out = open(file, O_CREAT|O_APPEND|O_WRONLY, 0600); // opens file
    dup2(out, 1); // redirects stdout to go to the file
    close(out);
}