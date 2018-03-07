/*
 * errorPrinter.c
 *
 *  Created on: 7 mrt. 2018
 *      Author: Oberjarl
 */

#include "errorPrinter.h"

void errorPrinter(errorType theError){
#ifdef DEBUG
	printf("[Function]\t%s\r\n", __func__);
#endif

	printf("[Error]\t");
	switch(theError){
	case e_None:
		printf("None:\tThere was no error.\r\n");
		break;
	case e_Unspecified:
		printf("Unspecified:\tThere was an error, but it is unknown what went wrong.\r\n");
		break;
	default:
		printf("There was no error message specified for this.\r\n");
	}
}
