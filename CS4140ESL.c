/**
 * This project is for the CS4140 project entry.
 *
 * Author: 			Sander van Leeuwen
 * Student number:	4351436
 * System name:		Oberjarl
 */

// Config includes the standard, should be global, files.
#include "config.h"

#include "inputReader.h"
#include "errorPrinter.h"

/**
 * This is the main function.
 * No input arguments have been implemented.
 */
int main(int argc, char **argv) {
#ifdef DEBUG
	printf("[Path]\t%s\r\n", argv[0]);
	printf("Number of provided arguments:\t%d\r\n", argc-1);
	for(int i=1; i<argc; i++){
		printf(">\t%s\r\n",argv[i]);
	}
#endif

	errorPrinter(e_None);
	errorPrinter(e_Unspecified);

	return 0;
}
