#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>

extern char **get_line(void);


int run_external(char **args, char* input, char* output,  bool bg) {
  pid_t pid;
  pid = fork();
  if (pid > 0) {
    /* Then we are parent */
    int status;
    wait(&status);
  } else if (pid == 0) {
    
    /* Then we are child */
    if (input) {
      freopen(input, "r", "stdin");
    }
    if (output) {

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
  printf("\x1b[35;1m");
  printf(">> ");
  printf("\x1b[0m");
}

int count_array(char **args) {
  int c = 0;
  while (*args != NULL) {
    c++;
    args++;
  }
  return c;
}

void run_command(char **args) {
  /*bool bg = false;*/
  char *cmd = args[0];
  printf("argc: %d\n", count_array(args));
  if (strcmp(cmd,"exit") == 0) {
    exit(0);
  } else if (strcmp(cmd,"cd") == 0) {
    /* TODO needs error checking on input and chdir return */
    chdir(args[1]);
  } else {
    run_external(args, "test.txt", NULL, false);
  }
}

int main(int argc, char *argv[]) {
  int i; 
  char **args; 
  while(1) {
    print_chrome();
    args = get_line();
    if(!args) break;
    for(i = 0; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
    }

    run_command(args);
  }
  return 0;
}

