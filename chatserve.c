/*********************************************************************
** Author: James G Wilson
** Date: 5/1/2018
** Description: chatserve.c
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

void printUsage();

int main(int argc, char*argv[])
{
	
	/* Check command line bad usage.
	 * Only checks if the number of arguments is wrong. */
	if(argc != 2)
	{
		printUsage();
	}

	// Get port number from command line arg
	// Wait on that port for a client request
	
	// Taken from Beej's Guide to Network Programming :D
	// Set up server to listen on this host's IP address, port ####
	int status;
	struct addrinfo hints;		// fill out with relevent info
	struct addrinfo *servinfo;	// will point to results

	memset(&hints, 0, sizeof(hints));	// make sure the struct is empty
	hints.ai_family = AF_UNSPEC;		// don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets (woot!)
	hints.ai_flags = AI_PASSIVE;		// fill in my IP for me
	
	if((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	// servinfo now points to a linked list of 1 or more struct addrinfos
	
	// . . . do everything until you don't need servinfo anymore . . . .
	
	freeaddrinfo(servinfo); // free the linked-list

	return 0;
}

/* Print command line argument format */
void printUsage()
{ 
	printf("Usage ./chatserve <port#>\n");
}

