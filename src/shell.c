/* shell.c
 * Contains the main code for shell functionality.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* MAX_PATH_LENGTH is the longest path name supported (used for print_chrome)
 * MAX_COMMANDS is the longest number of arguments allowed for one command
 */
#define MAX_PATH_LENGTH 256
#define MAX_COMMANDS 256

extern char **get_line(void);

/* This function forks and executes a new process, input is a possible input
 * redirection (NULL is none), output is a possible output redirection,
 * and bg is true for a background process. 
 */
int run_external(char **args, char* input, char* output,  bool bg) {
  pid_t pid; /* Will be the pid of the fork result */
  pid = fork();
  if (pid > 0) {
    /* Then we are parent */
    int status;
    pid_t reaped_pid;
    if (!bg) { 
      /* This is the loop to reap zombies */
      do {
        reaped_pid = wait(&status);
        if (reaped_pid != pid) {
          printf("%d: background process terminated\n", reaped_pid); 
        }
      } while (reaped_pid != pid);
    }
    /* Clear up zombies */
  } else if (pid == 0) {
    
    /* Then we are child */
    if (input) {
      freopen(input, "r", stdin);
    }
    if (output) {
      freopen(output, "w", stdout);
    }

    execvp(args[0], args);

    /* At this point there was an error in execvp */
    fprintf(stderr, "Error opening: %s.", args[0]);
    
  } else {
    fprintf(stderr, "Error forking a child process");
  }
}

/* Prints the prompt with current directory and colors */
void print_chrome(void) {
  /* switch color */
  char cwd[MAX_PATH_LENGTH];
  if (getcwd(cwd,MAX_PATH_LENGTH)) {
    printf("\x1b[35;1m");
    printf("%s \x1b[32;1m>> ", cwd);
    printf("\x1b[0m");
  } else {
    printf("\x1b[32;1m>> \x1b[0m");
  }
}

/* Takes a list of arguments an changes the working directory to
 * argument 1, checking for errors 
 */
change_dir(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "Usage: cd path\ncd requires a path\n");
  } else {
    if (chdir(args[1]) < 0) {
      fprintf(stderr, "Cannot cd into %s\n", args[1]);
    }
  }
}

/* Takes a list of arguments, gets all the flags and shell
 * commands, and if required sends an array of the rest of
 * the arguments to run_external()
 */
void run_command(char **args) {
  char *new_args[MAX_COMMANDS]; /* Holds filtered arguments */
  char **new_args_i = new_args; /* Used as a pointer in loop */
  
  char *cmd; /* Will be pointer to the first argument */

  /* "Flags" for shell commands */
  char *input = NULL;  /* Input redirection, NULL if none */
  char *output = NULL; /* Output redirrection, NULL is none */
  bool bg = false;     /* Should this command run in the background */

  /* Check for an empty command */
  if (*args == NULL) {
    return;
  }

  /* Loop through all arguments */
  while (*args != NULL) {
    if (strcmp(*args,"<") == 0) {
      args++;
      if (*args) {
        input = *args;
      } else {
        break;
      }
    } else if (strcmp(*args,">") == 0) {
      args++;
      if (*args) {
        output = *args; 
      } else {
        break;
      }
    } else if (strcmp(*args,"&") == 0) {
      bg = true;
    } else {
      *new_args_i = *args;
      new_args_i++; 
    }
    args++;
  }
  *new_args_i = NULL;

  /* Decide what to do */
  cmd = new_args[0];
  if (strcmp(cmd,"exit") == 0) {
    exit(0);
  } else if (strcmp(cmd,"cd") == 0) {
    change_dir(new_args);
  } else {
    run_external(new_args, input, output, bg);
  }
}

/* Entry point, simple input loop */
int main(int argc, char *argv[]) {
  int i; 
  char **args; 
  while(1) {
    print_chrome();
    args = get_line();
    /* Checks end of file */
    if(!args) break;
    /* Used for debugging
    for(i = 0; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
    }
    */
    run_command(args);
  }
  return 0;
}

