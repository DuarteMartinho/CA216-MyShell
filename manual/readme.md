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

    HELP
        DESCRIPTION
            This is the help command that shows this manual

        USAGE
            help

    CD
        DESCRIPTION
            This command will change directory if arg1 is given, else it will just print the current working directory.

        USAGE
            cd <arg1>
                OR
            cd

    DIR
        DESCRIPTION
            This command will list all the directories and files in the folder name or path given
        USAGE
            dir <folder name>
                OR
            dir <path to folder>

    ENVIRON
        DESCRIPTION
            This command will print all the environment variables

        USAGE
            environ

    PAUSE
        DESCRIPTION
            This command will pause the shell until the user presses ENTER
        
        USAGE
            pause

    ECHO
        DESCRIPTION
            This command will echo all the args to standard output or to a output file given by I/O redirection

        USAGE
            echo <args> 
                OR 
            echo <args> + I/O redirection

    QUIT
        DESCRIPTION
            This command will exit the shell

        USAGE
            quit

    OTHER COMMANDS
        DESCRIPTION
        This shell supports any other commands to be used with a max args of 64.
        All of these commands support I/O redirection from a file and to a file.

        USAGE
            <programme> <arg1> <arg2> <arg3> ...  <last_arg>

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
    All the following references are resources that I have used to create myshell.
    1. environ - man page: https://man7.org/linux/man-pages/man7/environ.7.html
    2. chdir - man page: https://man7.org/linux/man-pages/man2/chdir.2.html
    3. putenv - man page: https://man7.org/linux/man-pages/man3/putenv.3.html
    4. fputs - man page: https://man7.org/linux/man-pages/man3/fputs.3p.html
    5. fork - man page: https://man7.org/linux/man-pages/man2/fork.2.html
    6. exec - man page: https://man7.org/linux/man-pages/man3/exec.3.html
    7. open - man page: https://man7.org/linux/man-pages/man2/open.2.html


//----------------------------------------------------------------
// Name: Duarte Martinho
// Student Number: 20410974
// I acknowledge DCU's Academic Integrity Policy