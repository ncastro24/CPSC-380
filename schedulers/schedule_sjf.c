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

// SHORTEST JOB FIRST //
// Processes are executed based off which has the shortest job //
// associate with each process the length of its next cpu burst //
// must run task with shortest burst first -- see if next node val is smaller //

//used functions from list.c, cpu.c, schedulers

struct node *sjfhead;

void add(char* n, int p, int b){
  struct task *sjf = malloc(sizeof(struct task));
  sjf->name = n;
  sjf->priority = p;
  sjf->burst = b;
  insert(&sjfhead, sjf);
}

struct node* checkshortest(struct node *sc, struct node *sh){

  // program runs 'sh' (shortest), which is first set equal to the original node 'sc' (scheduled)
  // program checks if the original node 'sc's burst time is less than 'sh'
    // if it is, the new 'sh' node becomes the current value of 'sc's node
  // if the current value of 'sc' isnt less than the sh burst,
    // move onto the next value of 'sc' and check again until end of list

  while (sc != NULL){
    if (sc->task->burst < sh->task->burst){
      sh = sc;
    }
    //should priority be taken into account if they have the same burst time?
    //check: [might not be needed]
    /* else if (sc->task->burst == sh->task->burst){
      if (sc->task->priority < sh->task->priority){
        sh = sc;
      }
    }*/
    else{
      sc = sc->next;
    }
  }

  return sh;
}

void schedule(){

  while (sjfhead != NULL){

    struct node *scheduled = sjfhead;
    struct node *shortest = scheduled;
    struct node *temp = checkshortest(scheduled, shortest);

    run(temp->task, temp->task->burst);
    delete(&sjfhead, temp->task);


  }

}
