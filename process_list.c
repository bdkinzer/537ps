// Authors: Ben Kinzer, 9073435027, bkinzer@wisc.edu, ben
//          Austin Castleberry, 9074592214, acastleberry@wisc.edu, castleberry

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "process_list.h"

// Counts number of processes in /proc to know how big of an array of pids to allocate
int procSize(void) {
    // number of processes
    int dirSize = 0;
    // single entry in proc table
    struct dirent *entry;
    // directory to be opened
    DIR *dir = opendir("/proc");

    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            dirSize++;
        }	    
        if (closedir(dir) != 0) {
            printf("Directory could not be closed.\n");
	    exit(1);
        }
    } else {
        printf("Directory could not be opened.\n");
	exit(1);
    }

    return dirSize; 
}

// creates and returns a list of pids of processes in /proc owned by the user takes list of options chosen
// under flagArr and the number of processes under dirSize
int* listProc(int* flagArr, int dirSize){
    // current user's id
    int userid = getuid();
    // array of pid's for the processes we want to display
    int *pidArr = (int*) calloc(sizeof(int), dirSize);
    // pointer for each entry of the directory
    struct dirent *entry;
    // directory to look for processes in
    DIR *dir = opendir("/proc");

    if (dir) {
	// if the first element of the flag array is 0, get the process list, if 1,
	// make sure the pid given by the user is in /proc
        if (*(flagArr) == 0) {
	    // file with user id information for each process
	    FILE *statusFile;
            // next index for array of pid's     
	    int i = 0;
	    // line in 'status' file of a proc that contains the user id
	    int uidline = 8;
	    //const int MAX_DIRECTORY_SIZE;
	    const int LINE_SIZE = 100;
	    // path to the status
	    char path[32];
	    // one line in statusFile
            char line[LINE_SIZE];
	    // number of lines in file already searched through
            int count = 0;
	    // pid of the current process we are looking
            int currPid;
	    // user id of the current process we are looking in
	    int procUid;

	    while ((entry = readdir(dir)) != NULL) {
                // gets current pid in /proc
		currPid = atoi(entry->d_name);
	        if(currPid == 0) { 
	            continue;
	        }
		
	        // gets the path to the status file of the current pid and opens path
                snprintf(path, sizeof(path), "/proc/%d/status", currPid);
		statusFile = fopen(path, "r");

	        // if file cannot be opened exi
	        if (statusFile == NULL){
	            printf("Error opening status file for process: %d\n", currPid);
		    exit(0);
	        }

		// prepares line variable for reading status file
	        memset(&line[0], 0, sizeof(line));
                // string just in front of uid should be Uid:
                char stringHlder[16];

		// goes through each line of status file until uid is found
	        while(fgets(line, sizeof(line), statusFile)) {
	            if (count == uidline){
			// when uid is found, put it in procuid
                        sscanf(line, "%s %d", stringHlder, &procUid);
			
			// if the uid of the process is the same as the user's the process is put into
			// the list of processes to print information of
                        if (procUid == userid){
		            *(pidArr + i) = currPid;
			    i++;
	                }

			count = 0;
			break;
		    }
	            count++;	   	
	        }
	    }

	    if (closedir(dir) != 0) {
                printf("Directory could not be closed.\n");
		exit(1);
            }
        } else {
	    // gets single pid if -p option was chosen
            while ((entry = readdir(dir)) != NULL) {
                if (atoi(entry->d_name) == *(flagArr + 1)) {
                    *(pidArr) = *(flagArr + 1);
                    if (closedir(dir) != 0) {
                        printf("Directory could not be closed.\n");
			exit(1);
		    }
	        }
	    }	
        }
    // error list
    } else if (errno == ENOENT) {
        printf("This directory does not exist.");
	exit(1);
    } else if (errno == ENOTDIR) {
        printf("This file is not a directory.");
        exit(1);
    } else if (errno == EACCES) {
        printf("You do not have the right to open /proc.");
	exit(1);
    } else {
        printf("/proc could not be opened due to an unexpected error, check the manual.");
	exit(1);
    }

    return pidArr;    
} 
