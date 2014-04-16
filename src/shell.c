#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 256
#define MAX_COMMANDS 256

extern char **get_line(void);


int run_external(char **args, char* input, char* output,  bool bg) {
  pid_t pid;
  pid = fork();
  if (pid > 0) {
    /* Then we are parent */
    int status;
    pid_t reaped_pid;
    if (!bg) { 
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
    /* TODO Error */
  }
}

void print_chrome(void) {
  /* switch color */
  char cwd[MAX_PATH_LENGTH];
  getcwd(cwd,MAX_PATH_LENGTH);
  printf("\x1b[35;1m");
  printf("%s \x1b[32;1m>> ", cwd);
  printf("\x1b[0m");
}

void run_command(char **args) {
  char *new_args[MAX_COMMANDS];
  char *cmd;
  char *input = NULL;
  char *output = NULL;
  bool bg = false;
  
  char **new_args_i = new_args;
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

  cmd = new_args[0];
  if (strcmp(cmd,"exit") == 0) {
    exit(0);
  } else if (strcmp(cmd,"cd") == 0) {
    /* TODO needs error checking on input and chdir return */
    chdir(new_args[1]);
  } else {
    run_external(new_args, input, output, bg);
  }
}

int main(int argc, char *argv[]) {
  int i; 
  char **args; 
  while(1) {
    print_chrome();
    args = get_line();
    if(!args) break;
    /*
    for(i = 0; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
    }
    */
    run_command(args);
  }
  return 0;
}

