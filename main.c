// Authors: Ben Kinzer, 9073435027, bkinzer@wisc.edu, ben
//          Austin Castleberry, 9074592214, acastleberry@wisc.edu, castleberry

#include "options_processing.h"
#include "stat_file_parser.h"
#include "process_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

// standard function for <= to compare two numbers for quicksort. 
int compare(const void *a, const void *b) {
    return (*(int*) a - *(int*) b);
}

// gets options from user. Gets processes designated by user. Sorts these processes. Finally displays
// information designated by user
int main(int argc, char* argv[]) {
    // array of options from the command line
    int* options;
    options = optionsProcessing(argc, argv);
    // number of processes in /proc;    
    int dirSize; 
    dirSize = procSize();
    // array of process ids
    int *pidArrPtr = listProc(options, dirSize);
    qsort(pidArrPtr, 7, sizeof(int), compare);
   
    // current pid to print
    int currPid;
    // goes through each pid and prints necessary information
    for (int i = 0; i < dirSize; i++) {
        if (*(pidArrPtr + i) != 0) {
	    currPid = *(pidArrPtr + i); 
        
            if (printProcess(currPid, options) == 0) {
                printf("Error in printing process info for process: %d", currPid);
            }
	}
    }
    
    free(pidArrPtr);
    free(options);
}
