// Authors: Ben Kinzer, 9073435027, bkinzer@wisc.edu, ben
//          Austin Castleberry, 9074592214, acastleberry@wisc.edu, castleberry

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "options_processing.h"

// Parses command line and changes flags in optionsArray depending on which options chosen. 1 shows
// information, 0 hides it
int* optionsProcessing(int argc, char* argv[]) {
    // array of all option flags to be sent to main of format {p,pid,s,U,S,v,c}
    int* optionsArray = malloc(sizeof(int) * 7);
    if (optionsArray == NULL) {
        printf("Error with memory allocation.\n");
    }
    // defaults options U and c to true
    *(optionsArray + 3) = 1;
    *(optionsArray + 6) = 1;

    // Current argument in command line
    char option;
    option = getopt(argc, argv, "p:s::U::S::v::c::");
    
    // Goes through each argument in command line and sets the option to true or false and gets pid for -p
    while (option != -1) {
        switch (option) {
            case 'p':
                *(optionsArray) = 1;
		// Checks if option has trailing - which signifies shutting option off
		if (*optarg == '-') {
                    *(optionsArray) = 0;
		    break;
		}
		*(optionsArray + 1) = atoi(optarg);
		break;
	    case 's':
		*(optionsArray + 2) = 1;
		if (optarg != 0 && *optarg == '-') {
                    *(optionsArray + 2) = 0;
		}
		break;
	    case 'U':
		*(optionsArray + 3) = 1;
		if (optarg != 0 && *optarg == '-') {
                    *(optionsArray + 3) = 0;
		}
		break;
	    case 'S':
		*(optionsArray + 4) = 1;
		if (optarg != 0 && *optarg == '-') {
                    *(optionsArray + 4) = 0;
		}
		break;
	    case 'v':
		*(optionsArray + 5) = 1;
		if (optarg != 0 && *optarg == '-') {
                    *(optionsArray + 5) = 0;
		}
		break;
	    case 'c':
		*(optionsArray + 6) = 1;
		if (optarg != 0 && *optarg == '-') {
                    *(optionsArray + 6) = 0;
		}
		break;
	    default:
		printf("Unrecognized option.\n");
		exit(1);
        }

	// Gets next argument
	option = getopt(argc, argv, "p:s::U::S::v::c::");
    }

    if (*(optionsArray) == 1 && *(optionsArray + 1) < 1) {
        printf("Not a valid pid\n");
	exit(1);
    }

    return optionsArray;
}
