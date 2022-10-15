/*
Nayeli Castro
2340374
CPSC 380-01
Assignment 4: scheduler
References:
Ch 5 notes
https://www.edureka.co/blog/first-come-first-serve-scheduling/#:~:text=First%20Come%20First%20Serve%20is,are%20requested%20by%20the%20processor.
https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c
https://stackoverflow.com/questions/17028974/c-calling-linked-list-function-from-another-file
https://stackoverflow.com/questions/15441745/explanation-of-code-linked-list-c
*/

// FIRST COME FIRST SERVE//
// Processes are executed in the order they come in //
// no need to sort items in array //

//used functions from list.c, cpu.c, schedulers

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
//#include "driver.c"
#include "cpu.h"

struct node *fcfshead;

void add(char* n, int p, int b){
  struct task *fcfs = malloc(sizeof(struct task));
  //struct node *fcfshead;
  //fcfshead->task = malloc(sizeof(struct node));
  fcfs->name = n;
  fcfs->priority = p;
  fcfs->burst = b;
  insert(&fcfshead, fcfs);
}

void schedule(){

  while (fcfshead != NULL){
    struct node *scheduled = fcfshead;
    run(scheduled->task, scheduled->task->burst);
    delete (&fcfshead, scheduled->task); //end task
  }

}
