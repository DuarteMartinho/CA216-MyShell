NAME
    myshell - example shell

DESCRIPTION
    myshell is a UNIX command-line shell where you can execute commands. 
    myshell has a custom command-line commands and any other shell functions, also has a batchfile mode, I/O redirection and other features.

START/SHUTDOWN
    1. Go into `src` directory
    2. Open your current command-line and run `make`
    3. After thats done go into the `bin` directory
    4. And in your current command-line run `./myshell`

COMMANDS
    - help: display the user manual using the more filter
    - cd: change the current working directory
    - dir: list tge contents of a given directory
    - environ: list all environment variables
    - pause: pauses the shell untill `ENTER` is pressed
    - echo: echo on display the args passed in
    - quit: quit the shell
    - Any Other OS Commands

ENVIRONMENT
    Environment values are variables normally set by the shell/OS. It is composed by a "NAME=VALUE" the value is normally a string.

    In myshell you can run the command `environ` to get all environment variables on the current machine to the stdout.

    In myshell it has set the SHELL environment variable to the path of the shell executable.

BATCHFILE
    myshell can take commands from a txt file that contains a list of commands which will be executed in the order they are in. When the end-of-file is reached, the shell will exit.

    USAGE
        ./myshell `batchfile`

I/O REDIRECTION
    myshell supports Input/Output redirection and it supports either stdin or stdout.

    USAGE
        ./myshell `arg1` `arg2` < `inputfile` > `outputfile`
    
    OPTIONS
        < - Then the inputfile is taken from the next argument 
        > - Then the outputfile is taken from the next argument and is created if it does not exist and truncated if it does
        >> - Then outputfile is taken from the next argument and is created if it does not exist and appended to if it does


BACKGROUND/FOREGROUND EXECUTION
    myshell supports background and foreground execution of programs. 

    Background execution is when the shell returns to the command line prompt immediately while it is running the command in the background.
    
    USAGE
        An ampersand, or & character, at the end of the command line
        <command> <args> &
    

    Foreground execution is when the commands run as usual. The shell will wait for the command to finish before continuing.

    USAGE
        <command> <args>

    The main difference between the two is that the foreground gives somewhat visual feedback to the user while the background does not.

AUTHOR
    myshell was developed by Duarte Martinho.

REFERENCES
    1. environ - man page: https://man7.org/linux/man-pages/man7/environ.7.html
    2. chdir - man page: https://man7.org/linux/man-pages/man2/chdir.2.html
    3. putenv - man page: https://man7.org/linux/man-pages/man3/putenv.3.html
    4. fputs - man page: https://man7.org/linux/man-pages/man3/fputs.3p.html
    5. fork - man page: https://man7.org/linux/man-pages/man2/fork.2.html
    6. exec - man page: https://man7.org/linux/man-pages/man3/exec.3.html


//----------------------------------------------------------------
// Name: Duarte Martinho
// Student Number: 20410974
// I acknowledge DCU's Academic Integrity Policy