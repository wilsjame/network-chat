/*********************************************************************
** Author: James G Wilson
** Date: 5/4/2018
** Description: chatclient.c
** Direct copy of chatserve.c for now
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 10 // Pending connections queue will hold
void printUsage();

int main(int argc, char*argv[])
{
	
	/* Check command line bad usage.
	 * Only checks if the number of arguments is wrong. */
	if(argc != 2)
	{
		printUsage();
		exit(1);
	}
	
	// ***System call sequence***
	// getaddrinfo()
	// socket()
	// bind
	// listen
	// accept

	struct addrinfo hints;		// Fill out with relevent info
	struct addrinfo *result, *rp;	// Will point to results
	int sockfd, status, new_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	int yes = 1;

	memset(&hints, 0, sizeof(hints));	// Make sure the struct is empty
	hints.ai_family = AF_UNSPEC;		// Don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets (woot!)
	hints.ai_flags = AI_PASSIVE;		// Fill in my IP for me
	hints.ai_protocol = 0;			// Any protocol
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	// Set up the address struct for this process (the server)
	status = getaddrinfo(NULL, argv[1], &hints, &result);
	if(status != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	// getaddrinfo returns a linked-list of address structures, rp.
	// Try each address until a succesful bind().
	// If socket() (or bind()) fails, close the socket 
	// and try the next address.
	for(rp = result; rp != NULL; rp = rp->ai_next)
	{
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); // Create socket

		if(sockfd == -1);
		{
			fprintf(stderr, "Server: socket error\n");
			//continue; Cause of bug >:)
		}

		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			fprintf(stderr, "Server: setsockopt error\n");
			exit(EXIT_FAILURE);
		}

		if(bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) // Bind socket to port #
		{
			printf("bind success!\n");
			break; // Success!
		}
		else
		{
			fprintf(stderr, "Server: bind error\n");
		}

		close(sockfd);
	}

	if(rp == NULL) // No address succeeded
	{
		fprintf(stderr, "Server: failed to bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result); // Free the linked-list, no longer needed

	// Begin queuing connection requests until limit is reached.
	// It can receive up to BACKLOG connections.
	listen(sockfd, BACKLOG); 

	// Loop and accept clients indefinitely
	while(1)
	{
		
		// Accept a connection or block (if one is not available) until one connects.
		// accept() creates a new socket to be used for actual communication. 
		// sockfd continues listening!
		addr_size = sizeof(client_addr);
		new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);

		if(new_fd < 0)
		{
			fprintf(stderr, "Server error on accept\n");
		}

	}
		 
	return 0;

}

/* Print command line argument format */
void printUsage()
{ 
	printf("Usage ./chatserve <port#>\n");
}

