/*
Nayeli Castro
2340374
CPSC 380-01
Assignment 5: prodcon

NOTE:
- I am on mac, so I will use sem_open and sem_unlink

KNOWN ISSUES:
- I could not figure out how to put the timestamp and item number
- Loops forever but does not print out my test prints--unsure if
- program got to its destination.

References:
class notes
thread and shared memory example code from class (thread3.c, shmwrite.c, etc)
https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-mutexes
https://medium.com/helderco/semaphores-in-mac-os-x-fd7a7418e13b
https://www.geeksforgeeks.org/posix-shared-memory-api/
https://www.tutorialspoint.com/c_standard_library/c_function_malloc.htm
https://stackoverflow.com/questions/3463976/c-file-checksum
https://www.geeksforgeeks.org/memcpy-in-cc/
https://www.geeksforgeeks.org/producer-consumer-problem-in-c/
*/

#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MEM_SIZE 64000 // max memory size
#define ITEM_SIZE 32 //total 32 byte item
#define RAND_SIZE 22 //22 bytes of random items

void *producer(void *arg);
void *consumer(void *arg);
int nitems;
int memsize;
int blocks;
char* memory;

int in = 0;

pthread_mutex_t mutex;
sem_t *sem0;
sem_t *sem1;
pthread_t prod; //producer thread
pthread_t con; //consumer thread

//temp checksum variables for both producer and consumer
//used to check if checksums match
unsigned short int prod_chksum;
unsigned short int con_chksum;

//timestamps
unsigned int zero_time; //timmestamp for sem0
unsigned int one_time; //timestamp for sem1

//item numbers
int zero_itemnum; //timestamp for sem0
int one_itemnum; //timestamp fro sem1

int main(int argc, char** argv){
  //error handling
  //nitems cant be negative
  if (atoi(argv[1]) < 0){
    printf("Number of items must be nonnegative!\n");
    return -1;
  }
  else{
    printf("Successful\n");
  }

  nitems = atoi(argv[1]);
  blocks = nitems / ITEM_SIZE; //amount of blocks utilized
  memsize = blocks*ITEM_SIZE;
  //int buffer[nitems];
  memory = malloc(memsize);

//start with semaphore unlink for mac - ignore error
  sem_unlink("/sem0");
  sem_unlink("/sem1");

  sem0 = sem_open("/sem0", O_CREAT, 0644, 1);
  sem1 = sem_open("/sem1", O_CREAT, 0644, 1);
  printf("Opened semaphores!");

  //zero_time = time.time();
  //one_time = time.time();

  //initialize semaphores
  if (sem0 == SEM_FAILED || sem1 == SEM_FAILED) {
    printf("Failed to open semaphore.");
    return -1;
  }

//initialize threads

  pthread_mutex_init(&mutex, NULL);

  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&con, NULL, consumer, NULL);
  printf("Created threads!");

  pthread_join(prod, NULL);
  pthread_join(con, NULL);
  printf("Joined threads!");

  sem_close(sem0);
  sem_close(sem1);
  printf("Closed semaphores!");

  sem_unlink("/sem0");
  sem_unlink("/sem1");

  free(memory); // free the memory when its done
  printf("Memory freed");

  pthread_mutex_destroy(&mutex);

return 0;
}

//check if producer and consumer checksums match
void checksumCheck(int p, int c){
  if (p != c){
    printf("Checksum doesn't match!\n");
    exit(EXIT_FAILURE);
  }
  else{
    printf("Checksums match!\n");
  }
}

//from checksum.c example code from class
uint16_t checksum(char *addr, uint32_t count)
{
    register uint32_t sum = 0;

    uint16_t *buf = (uint16_t *) addr;

    // Main summing loop
    while(count > 1)
    {
        sum = sum + *(buf)++;
        count = count - 2;
    }

    // Add left-over byte, if any
    if (count > 0)
        sum = sum + *addr;

    // Fold 32-bit sum to 16 bits
    while (sum>>16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return(~sum);

}

//The producer thread is to create the data item(s) which includes the item number,
//timestamp, checksum and random data.

//semaphore 0

void *producer(void *arg){
  int item;
  char prod_array[ITEM_SIZE];

  for (int i = 0; i < nitems; i++){
    printf("Made it to producer!");

    sem_wait(sem0);

    if (sem_wait(sem0) != 0){ //check if semaphore link successful
      printf("Semaphore failed.");
      exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex);
    item = rand();
    prod_array[i] = item;
    memory[i] = prod_array[i];
    //pthread_mutex_lock(&mutex);

    prod_chksum = checksum(&prod_array[0], RAND_SIZE); //check sum for 22 random bytes

    pthread_mutex_unlock(&mutex); //unlock to be able to go to the next semaphore

    sem_post(sem1);
    if (sem1 == SEM_FAILED) {
      printf("Failed to open semaphore.");
      exit(EXIT_FAILURE);
    }
  }
pthread_exit(0);
}

void *consumer(void *arg){
  int item;
  char con_array[ITEM_SIZE];

  for (int i = 0; i < nitems; i++){
    printf("Made it to consumer!");
    sem_wait(sem1);

    if (sem_wait(sem1) != 0){ //check if semaphore link successful
      printf("Semaphore failed.");
      exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&mutex);

    item = rand();
    con_array[i] = item;

    con_chksum = checksum(&con_array[0], RAND_SIZE);

    checksumCheck(prod_chksum, con_chksum);

    pthread_mutex_unlock(&mutex);

    sem_post(sem0);
    if (sem0 == SEM_FAILED) {
      printf("Failed to open semaphore.");
      exit(EXIT_FAILURE);
    }

  }
  pthread_exit(0);
}
