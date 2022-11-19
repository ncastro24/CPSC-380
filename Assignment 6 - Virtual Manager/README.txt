Nayeli Castro
2340374
CPSC 380-01
Assignment 6: vmmgr Virtual Address Manager

References:
https://www.cplusplus.com/reference/cstdio/fopen/
https://www.tutorialspoint.com/c_standard_library/c_function_fscanf.html
https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c
Class notes

Known Issues:
- Counts hits incorrectly
- Backing store page fault implementation is incorrect/unfinished

Compile:
gcc -o vmmgr vmmgr.c
./vmmgr addresses.txt
