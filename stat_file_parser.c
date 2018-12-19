// Authors: Ben Kinzer, 9073435027, bkinzer@wisc.edu, ben
//          Austin Castleberry, 9074592214, acastleberry@wisc.edu, castleberry

#include <stdio.h>

// prints info for a single process. Uses flagArr for which info to print and pid for which process to print
// returns 1 if successful, 0 if not
int printProcess(int pid, int *flagArr) {
    // various flags in flag array
    int s = *(flagArr + 2);
    int U = *(flagArr + 3);
    int S = *(flagArr + 4);
    int v = *(flagArr + 5);
    int c = *(flagArr + 6);

    // path to appropriate file for information
    char path[50];
    // information in file
    char buffer[512];
    // information used for s flag
    char stateInfo;
    // information used for the v flag 
    int numPages;
    // utime is informatin used by the U flag, stime is information used by the S flag
    unsigned long int utime, stime;
    // information for the c flag
    char cmdline[256];

    // opens stat file
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    // pointer to file to be opened
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("Could not open file at: %s", path);
	return 0;
    }

    fgets(buffer, 512, fptr);

    // waste variables are bits of the file that will be unused
    int waste2;
    char waste3[100];
    unsigned int waste4;
    unsigned long int waste;

    sscanf(buffer, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu", &waste2, waste3, &stateInfo, 
        &waste2, &waste2, &waste2, &waste2, &waste2, &waste4, &waste, &waste, &waste, &waste, &utime, &stime);

    // prints info for s, U, and S flags
    printf("%d: ", pid);

    if (s == 1) {
        printf("%c ", stateInfo);
    }
    
    if (U == 1) {
	printf("utime=%lu ", utime);
    }
    
    if (S == 1) {
        printf("stime=%lu ", stime);
    }
    
    fclose(fptr);

    // prepares next file to read
    snprintf(path, sizeof(path), "/proc/%d/statm", pid);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("Could not open file at: %s", path);
        return 0;
    }
    
    fgets(buffer, 512, fptr);
    sscanf(buffer, "%d", &numPages);

    // prints v option
    if (v == 1) {
        printf("vmemory=%d ", numPages);	
    }

    fclose(fptr);

    // prepares next file and reads information
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);

    fptr = fopen(path, "r");

    if (fptr == NULL) {
        printf("Could not open file at: %s\n", path);
        return 0;
    }

    fgets(buffer, 512, fptr);

    sscanf(buffer, "%[^\n]s", cmdline);

    // prints c option
    if (c == 1) {
        printf("[%s]", cmdline);
    }   

    printf("\n");

    return 1;
}

