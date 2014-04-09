#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

extern char **get_line(void);

int run_external(char **args) {
  pid_t pid;
  pid = fork();
  if (pid > 0) {
    /* Then we are parent */
    int status;
    wait(&status);
  } else if (pid == 0) {
    /* Then we are child */
    char *p = "";
    execvp(args[0], args);
  } else {
    /* TODO Error */
  }
}

void print_chrome() {
  printf(">> ");
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
    if (strcmp(args[0],"exit") == 0) {
      exit(0);
    }
    run_external(args);
  }
  return 0;
}

