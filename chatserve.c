/*********************************************************************
** Author: James G Wilson
** Date: 5/1/2018
** Description: chatserve.c
*********************************************************************/
#include <stdio.h>

void printUsage();

int main(int argc, char*argv[])
{
	
	/* Check command line bad usage.
	 * Only checks if the number of arguments is wrong. */
	if(argc != 2)
	{
		printUsage();
	}

	return 0;
}

/* Print command line argument format */
void printUsage()
{ 
	printf("Usage ./chatserve <port#>\n");
}

