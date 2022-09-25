// Nayeli Castro
// 2340374
// CPSC 380-01
// Assignment 2
// References used:
/**
Chapter 3 notes
https://man7.org/linux/man-pages/man3/exec.3.html
https://cplusplus.com/reference/cstdio/fgets/
https://cplusplus.com/reference/cstring/strcmp/
https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
https://stackoverflow.com/questions/28733026/how-to-use-exec-to-execute-commands-in-c-and-linux
https://stackoverflow.com/questions/41252808/string-compare-in-c-with-fgets
https://stackoverflow.com/questions/1330550/c-compare-char-array-with-string
https://stackoverflow.com/questions/46069789/how-does-execl-deal-with-bin-sh-in-linux
https://stackoverflow.com/questions/3985193/what-is-bin-sh-c
**/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

//USED TEMPLATE FROM ASSIGNMENT DOC//
#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void) {
  char *args[MAX_LINE/2 + 1]; /* max of 40 arguments */
  char userInput[MAX_LINE]; //c style string: list of chars
  bool should_run = true;
  pid_t pid;
  int status;

    while (should_run) {
      printf("osh>");
      fgets(userInput, MAX_LINE, stdin);

      if (strcmp(userInput, "quit\n") == 0 || strcmp(userInput, "exit\n") == 0){
        printf("Exited program.\n");
        should_run = false;
        return 1;
      }

      //printf("osh>");
      fflush(stdout);

      pid = fork();

      if (pid < 0){
        perror("Failed to fork.\n");
        return 1;
      }
      if (pid == 0){
        execl("/bin/sh", "sh", "-c", userInput, NULL);
        should_run = false; //breaks loop
      }
      if (pid > 0){
        if (strcmp(userInput, "&") == 0){ //if user inputs &....
          sleep(1);
          printf("Waiting.");
          wait(NULL); //... wait
          pid = wait(&status);
        }
      }

/**      switch(pid){
        case (-1): //error
          perror("Failed to fork.\n");
          return 1;
          break;
        case (0): //child
          printf("Test");
          execl(userInput, userInput, NULL);
          should_run = false; //breaks loop
          break;
        default: //parent
          if (strcmp(userInput, "&") == 0){ //if user inputs &....
            sleep(1);
            printf("Waiting.");
            wait(NULL); //... wait
            //pid = wait(&status);
          }
          break;
      }
      **/
   }

return 0;
}
