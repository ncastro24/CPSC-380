/*
Nayeli Castro
2340374
CPSC 380-01
Assignment 7: Disk Scheduler

References:
Class Notes
https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
https://www.geeksforgeeks.org/disk-scheduling-algorithms/

Known Issues: SSTF seems to output an incorrect value.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DISK_CYLINDERS 5000
#define CYLINDER_REQUESTS 1000

int fcfs(int pos, int *req){
  int curr = pos;
  int mv = abs(req[0]-curr);
  for (int i = 0; i < CYLINDER_REQUESTS-1; i++){
    mv += abs(req[i]-req[i+1]);
  }
  return mv;
}

int sstf(int pos, int *req){
  //int *nreq = sort(req);
  int mv = 0;
  int curr = pos;
  int *nreq = req; //copy
  int closestreq = nreq[0];
  int reqindex = 0;

  int diff = abs(curr - closestreq);

  for (int i = 0; i < CYLINDER_REQUESTS; i++){
    if (diff > abs(curr - nreq[i])){
      closestreq = nreq[i];
      reqindex = i;
    }
    mv += abs(curr - closestreq);
    curr = closestreq;
    mv += abs(curr - nreq[i]);
  }
  //mv += abs(curr - nreq[0]);
  return abs(mv);
}

int scan(int pos, int *req){
  //int *sreq = scan(req);
  int curr = pos;
  int mv = 0;
  int nearmax = DISK_CYLINDERS - 1;
  //int ncurr = sreq[pos];

  for (int i = pos; i < DISK_CYLINDERS; i++){
    for(int j = 0; j < CYLINDER_REQUESTS; j++){
      if (i == req[j]){
        mv += abs(curr - req[j]);
        curr = req[j];
      }
    }
  }
  mv = mv + nearmax;
  curr = nearmax;

  for (int k = 0; k < pos; k++){
    for (int l = 0; l < CYLINDER_REQUESTS; l++){
      if (k == req[l]){
        mv += abs(curr - req[l]);
        curr = req[l];
      }
    }
  }
return mv;
}

int cscan(int pos, int *req){
  int curr = pos;
  int mv = 0;
  int nearmax = DISK_CYLINDERS - 1;

  for (int i = pos; i < DISK_CYLINDERS; i++){
    for(int j = 0; j < CYLINDER_REQUESTS; j++){
      if (i == req[j]){
        mv += abs(curr - req[j]);
        curr = req[j];
      }
    }
  }
  mv = mv + (nearmax - curr);
  mv = mv + nearmax;
  curr = 0;

  for (int k = 0; k < pos; k++){
    for (int l = 0; l < CYLINDER_REQUESTS; l++){
      if (k == req[l]){
        mv += abs(curr - req[l]);
        curr = req[l];
      }
    }
  }
  return mv;
}


// Thought that look and clook was needed, so I wrote them. Didn't realize it wasn't required
// for the assignment, but I still kept them here.

/* int look(int pos, int *req){
  int *lreq = req;
  int curr = pos;
  int mv = 0;
  int ncurr = lreq[pos];

  for (int i = pos + 1; i < CYLINDER_REQUESTS; i++){
    curr = lreq[i];
    mv += abs(ncurr - curr);
    ncurr = curr;
  }

  for (int j = pos - 1; j >= 0; j--){
    curr = lreq[j];
    mv += abs(curr - ncurr);
    ncurr = curr;
  }
  return abs(mv);
}

//similar to look but immediately shifts back to the start
int clook(int pos, int *req){
  int *clreq = req;
  int curr = pos;
  int mv = 0;
  int ncurr = clreq[pos];

  for (int i = pos + 1; i < CYLINDER_REQUESTS; i++){
    curr = clreq[i];
    mv += abs(ncurr - curr);
    ncurr = curr;
  }

  for (int j = 0; j < pos; j++){
    curr = clreq[j];
    mv += abs(curr - ncurr);
    ncurr = curr;
  }
  return mv;

} */

int main (int argc, char* argv[]){

  char line[32];

  if (argc < 3)
    {
        printf("Must enter 2 arguments.\n");
        return -1;
    }

  int position = atoi(argv[1]);
	if (position < 0 || position >= DISK_CYLINDERS) {
		printf("Position must be non-negative and between 0-4999.\n");
		return -1;
	}

  FILE *cylinder_file = fopen(argv[2], "r");
	if(cylinder_file == NULL) {
		printf("Cylinder request file error.");
		return -1;
	}

  // allocate memory
  int *requests = (int*) malloc(CYLINDER_REQUESTS * sizeof(int));

	// read every line in file to put in array
	while(fgets(line, 32, cylinder_file)) {
		int c_address = atoi(line);
    for (int i = 0; i >= CYLINDER_REQUESTS; i++){
      requests[i] = c_address;
    }
	}

	printf("FCFS: %i\n", fcfs(position, requests));

	printf("SSTF: %i\n", sstf(position, requests));

	printf("SCAN: %i\n", scan(position, requests));

	printf("CSCAN: %i\n", cscan(position, requests));

	//printf("LOOK: %i\n", look(position, requests));

	//printf("CLOOK: %i\n", clook(position, requests));

  free(requests);
  fclose(cylinder_file);
  return 0;

}
