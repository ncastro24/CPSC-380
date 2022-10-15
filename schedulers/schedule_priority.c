/*
Nayeli Castro
2340374
CPSC 380-01
Assignment 4: scheduler
References:
Ch 5 notes
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

// PRIORITY SCHEDULING //
// Priority number assigned with each process//
// smaller integer higher priority //
// process allocated based off higher priority //
// similar to sjf but taking priority into account instead of burst time !!!! //

struct node *pshead;

void add(char* n, int p, int b){
  struct task *ps = malloc(sizeof(struct task));
  ps->name = n;
  ps->priority = p;
  ps->burst = b;
  insert(&pshead, ps);
}

struct node* checkpriority(struct node *sc, struct node *hi){

  // program runs 'hi' (highest), which is first set equal to the original node 'sc' (scheduled)
  // program checks if the original node 'sc's priority is greater (REPRESENTED BY SMALLER INT-- so tehcnically 'less than') than 'hi'
    // if it is, the new 'hi' node becomes the current value of 'sc's node
  // if the current value of 'sc' isnt greater than the hi priority,
    // move onto the next value of 'sc' and check again until end of list

  while (sc != NULL){
    if (sc->task->priority < hi->task->priority){
      hi = sc;
    }
    //should shortest burst be taken into account if they have the same priority?
    //check: [might not be needed]
    /*else if (sc->task->priority == hi->task->priority){
      if (sc->task->burst < hi->task->burst){
        hi = sc;
      }
    }*/
    else{
      sc = sc->next;
    }
  }

  return hi;
}

void schedule(){
  while (pshead != NULL){

    struct node *scheduled = pshead;
    struct node *highest = scheduled;

    struct node *temp = checkpriority(scheduled, highest);

    run(temp->task, temp->task->burst);
    delete(&pshead, temp->task);


  }

}
