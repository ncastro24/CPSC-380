// Nayeli Castro
// 2340374
// CPSC 380-01
// Assignment 2
// References used:
/**
Chapter 4 notes
thrd-posix.c code provided in class for syntax help
https://www.geeksforgeeks.org/insertion-sort/
https://www.geeksforgeeks.org/c-program-to-merge-two-arrays/
https://www.cs.nmsu.edu/~rth/cs/cs271/notes/Pointers.html
https://stackoverflow.com/questions/47123166/passing-a-struct-to-a-thread-as-a-value-parameter
https://stackoverflow.com/questions/8442729/is-there-a-way-to-have-printf-properly-print-out-an-array-of-floats-say
https://cplusplus.com/reference/cstdlib/malloc/
https://cs.slu.edu/~dferry/courses/csci3500/studios/09_threads.html
https://stackoverflow.com/questions/58905195/multithreading-and-freeing-pointer-structs-in-c
**/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int testArr[]={7,12,19,3,18,4,2,6,15,8}; //template
int sortedArr[] = {};

//int n = sizeof(testArr); //does not work
int n = 10; //size of array

int i, j, k; //loop variables

void *sorter(void *param);
void *merger(void *param);

//================ PARAMETERS ================//

struct parameters {
   int left;
   int right;
};

//================ MAIN METHOD ================//

int main (int argc, char ** argv){

  pthread_t tid0; //1st half
  pthread_t tid1; //2nd half
  pthread_t tid2; //merged

//POINTERS: allocate memory for the threads
//PARAMETERS: struct using pointers made for reusability so I don't need to make more than-
//--1 sort method to handle each thread array half (each has a different left/right-
//--side of the original array)

//FIRST HALF OF ARRAY: split from start to middle of original array
  struct parameters *tid0Size = (struct parameters*)malloc(sizeof(struct parameters));
  tid0Size->left = 0;
	tid0Size->right = n/2;

//SECOND HALF OF ARRAY: split from middle to end of original array
  struct parameters *tid1Size = (struct parameters*)malloc(sizeof(struct parameters));
  tid1Size->left = n/2;
	tid1Size->right = n;

//THIRD ARRAY: combination of tid0 and tid1 from beginning to end
  struct parameters *tid2Size = (struct parameters*)malloc(sizeof(struct parameters));
  tid2Size->left = 0; //not needed
	tid2Size->right = n; //not needed

  pthread_create(&tid0,NULL,sorter,tid0Size); //sorted
	pthread_create(&tid1,NULL,sorter,tid1Size); //sorted
  pthread_create(&tid2,NULL,merger,tid2Size); //merged

  pthread_join(tid0, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

//print the sorted array
  for (i = 0; i < n; i++) {
    printf("%d ",testArr[i]);
  }
  return 0;
}

//================ SORT ================//

void insertionSort(int arr[], int start, int end){
 int tempArr;

 for (i = start; i < end; i++){
   tempArr = arr[i];
   j = i - 1;
   while (j >= 0 && arr[j] > tempArr){
     arr[j+1] = arr[j];
     j = j - 1;
   }
   arr[j+1] = tempArr;
 }
}

void *sorter(void *param)
{
  //gets what is the left and right side of each of the 2 threads' arrays
  //so its not needed to make 2 sorters for each param & just use one method for both
  struct parameters *tidInfo = (struct parameters *)param;
  int start = tidInfo->left;
  int end = tidInfo->right;

  insertionSort(testArr, start, end);
  //return NULL;
  pthread_exit(0);
}

//================ Merge ================//

void merge(int t3[],int left, int mid, int right){
  int firstHalf = mid - left + 1; //size of t0
  int secondHalf = right - mid; //size of t1

  int t0[firstHalf]; //array from 1st thread
  int t1[secondHalf]; //array from 2nd thread

   j = 0;
   k = 0;
  for(i = 0 ;i < n ;i++){
      if(t0[j] <= t1[k]){
         t3[i] = t0[j];
         j++;
      }
      else{
         t3[i] = t1[k];
         k++;
      }
   }
}

void *merger(void *param){
  merge(sortedArr, 0, n/2, n);
  //left: beginning of array value (0)
  //middle: midpoint of array
  //right: size of array
  pthread_exit(0);
}
