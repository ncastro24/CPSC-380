/*
Nayeli Castro
2340374
CPSC 380-01
Assignment 6: vmmgr Virtual Address Manager

References:
https://www.cplusplus.com/reference/cstdio/fopen/
https://www.tutorialspoint.com/c_standard_library/c_function_fscanf.html
https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c

Known Issues:
- Counts hits incorrectly, possibly because it never finds the table page to be the same as the page number
- Backing store page fault implementation is incorrect/unfinished
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
//#include <iostream>

#define BUFFER_SIZE 256
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define FRAME_SIZE 256

FILE *addressesFile;
FILE *backingStore;

double pf = 0.0; //page fault
double pfr = -1.0; //page fault rate
double hits = 0.0; //tlb hits
double hitrate = -1.0; //tlb hit rate

int pagetable[PAGE_SIZE];
char physicalmem[PAGE_SIZE * FRAME_SIZE]; //physical memory

typedef struct {
    short page;
    short frame;
} TLB;

TLB tlb[TLB_SIZE];
//TLB* tlb = (TLB*) malloc(TLB_SIZE* sizeof(TLB));

int main (int argc, char* argv[]){

  if(argc < 1)
	{
		printf("Please provide a valid file.\n");
		return -1;
	}

  char *fileName = argv[1];

  addressesFile = fopen(fileName, "r");
  if (addressesFile == NULL){
    printf("Error opening 'adddresses.txt'");
    return -1;
  }
  backingStore = fopen("BACKING_STORE.bin", "r");
  if (backingStore == NULL){
    printf("Error opening 'BACKING_STORE.bin'");
    return -1;
  }

  for (int j = 0; j < PAGE_SIZE; j++){
    pagetable[j] = -1;
  }
  /*for (int i = 0; i < TLB_SIZE; i++){
    tlb[i] = -1;
  }*/

  char* hex = (char*) malloc(4 * sizeof(char)); //allocate for hex conversion
  //char* memory = (char*) malloc(256 * 256 * sizeof(char)); //memory

  int rowcount = 0;

  int pagenum;
  off_t offset;
  int frame = 0;

  bool didhit = false;

  char buffer[32];
  //while (fscanf(addressesFile, "%d", &vaddr) == 1){
  while (fgets(buffer, 32, addressesFile)){
    int addr = atoi(buffer);
    rowcount++;
    pagenum = addr & 0xFFFF;
    pagenum = pagenum >> 8;
    offset = addr & 0xFF;

    for (int i; i < TLB_SIZE; i++){
      tlb[i].page = sprintf(hex, "%04X", tlb[i].page); //convert page into hex
      if (tlb[i].page == pagenum){ //if the table page hex is the same as the page number
        frame = tlb[i].frame;
        hits++;
        didhit = true;
        break;
      }
    }

    if (!didhit){ //if theres no hit create a page fault
      pf++;
      frame = pagetable[pagenum];
      fseek(backingStore, pagenum, SEEK_SET);
      int currentpage = frame*256;
      //fread(memory+currentpage, sizeof(char), 256, backingStore);
    }

  }

  printf("Table hits: %.1f\n", hits);
  printf("Table hit rate: %.2f\n", hitrate/rowcount);
  printf("Page faults: %.1f\n", pf);
  printf("Page fault rate: %.2f\n", pfr/rowcount);


  fclose(backingStore);
  fclose(addressesFile);
  //free(line);

  return 0;
}
