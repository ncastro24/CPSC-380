/*
Nayeli Castro
2340374
CPSC 380-01
Assignment 4: scheduler
References:
Ch 5 notes
https://www.geeksforgeeks.org/program-round-robin-scheduling-set-1/
https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c
https://stackoverflow.com/questions/17028974/c-calling-linked-list-function-from-another-file
https://stackoverflow.com/questions/15441745/explanation-of-code-linked-list-c
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
//#include "driver.c"
#include "cpu.h"

// ROUND ROBIN //
// Processes are executed based off a quantum time//
// must take quantum time into account when executing, //
// burst time must be less than time quantum //
// keep running every time quantum until the burst time runs out //

struct node *rrhead;

void add(char* n, int p, int b){
  struct task *rr = malloc(sizeof(struct task));
  rr->name = n;
  rr->priority = p;
  rr->burst = b;
  insert(&rrhead, rr);
}

int runtimeval(struct node *sc, int rt){ //method to get the amount of time task will run

  if (sc->task->burst >= QUANTUM){ //if the burst time is bigger than time quantum...
    rt = QUANTUM; //run time will be quantum
    sc->task->burst = sc->task->burst - QUANTUM; //subtract burst from quantum until its less
  }
  else if (sc->task->burst < QUANTUM){ //if burst time smaller than time quantum...
    rt = sc->task->burst; //just run with burst time
  }

  return rt;
}

void schedule(){
  int runtime = 0; //temp variable to store the amount of time task will run

  while (rrhead != NULL){

    struct node *scheduled = rrhead;

    runtime = runtimeval(scheduled, runtime);

    if (runtime < QUANTUM){ //delete task once its done with its runtime
      delete (&rrhead, scheduled->task);
    }
    if (runtime > 0){ //if runtime variable is initialized, run program using value in runtime variable
      run(scheduled->task, runtime);
    }
  }

}
