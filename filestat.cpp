// Nayeli Castro - CPSC-380-01 Assignment 1 - 09/14/2022
// references: previous C++ CPSC 350 work, stackoverflow, IBM documentation, linux.die.net

#include <iostream>
#include <pwd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
using namespace std;

int main (int argc, char ** argv){

  // OBJECTS //

  struct stat buffer;
  int status;
  string fileName; //name of user file

  int inodeNum;
  int fileType;
  int fileSize;
  long *date; //date last modified


// makes sure there is a valid terminal input //
  if (argc != 2) {
    printf("ERROR: Invalid terminal input.");
    exit(1);
  }

  //get file from input and use stat//
	fileName = argv[1];
		status = stat(fileName.c_str(), &buffer); // c_str converts file string into char

  if (stat(argv[1], &buffer) == -1) { //unsuccessful buffer
    printf("ERROR.");
    exit(1);
  }


// GET USER ID //
  struct passwd *userID;
  uid_t uid = 0; //ID valid if 0

  if ((userID = getpwuid(uid)) == NULL){ //check is userid is valid
    printf("ERROR: invalid username");
    exit(1);
  }

// GIVE VARIABLES THEIR VALUE //

    inodeNum = buffer.st_ino; //get inode number
    fileType = buffer.st_mode &S_IFMT; //get file type number
    fileSize = buffer.st_blksize; //get file size in bytes
    date = &buffer.st_mtime; //get date last modified

// CHECK FOR FILE TYPES//
    string fileTypeString;

    switch (fileType){
      case S_IFREG:
        fileTypeString = "Regular file";
        break;
      case S_IFDIR:
        fileTypeString = "Directory";
        break;
      case S_IFCHR:
        fileTypeString = "Character device";
        break;
      case S_IFBLK:
        fileTypeString = "Block device";
        break;
      case S_IFIFO:
        fileTypeString = "FIFO";
        break;
      case S_IFLNK:
        fileTypeString = "Symbolic link";
        break;
      case S_IFSOCK:
        fileTypeString = "Socket";
        break;
      default:
        fileTypeString ="Unknown file";
        break;
    }

    // OUTPUT //

    printf("Inode Number: %d\n", inodeNum);
    printf("File Type: %s\n", fileTypeString.c_str()); //turns filetype into a string
    printf("User: %s\n", userID -> pw_name); //turns id into name
    printf("File Size: %d\n", fileSize);
    printf("Date: %s\n", ctime(date)); //ctime converts date format

	return 0;

}
