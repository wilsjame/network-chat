/*********************************************************************
** Author: James G Wilson
** Date: 5/4/2018
** Description: chatclient.c
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 1 // Pending connections queue will hold
#define MAXDATASIZE 100

void printUsage();

int main(int argc, char*argv[])
{
	
	/* Check command line bad usage.
	 * Only checks if the number of arguments is wrong. */
	if(argc != 3)
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
	int sockfd, status, bytesReceived;
	//struct sockaddr_storage client_addr;
	//socklen_t addr_size;
	int yes = 1;
	char buffer[MAXDATASIZE];

	memset(&hints, 0, sizeof(hints));	// Make sure the struct is empty
	hints.ai_family = AF_UNSPEC;		// Don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets (woot!)
	//hints.ai_flags = AI_PASSIVE;		// Fill in my IP for me
	hints.ai_protocol = 0;			// Any protocol
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	// Set up the address struct for this process (the server)
	status = getaddrinfo(argv[1], argv[2], &hints, &result);
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
			fprintf(stderr, "Client: socket error\n");
			//continue; Cause of bug >:)
		}

		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			fprintf(stderr, "Client: setsockopt error\n");
		}

		if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) // connect socket to port #
		{
			printf("Client: connection to server success!\n");
			break; // Success!
		}
		else
		{
			fprintf(stderr, "Client: bind error\n");
		}

		// close bad socket and try again
		close(sockfd);
	}

	if(rp == NULL) // No address succeeded
	{
		fprintf(stderr, "Client: failed to bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result); // Free the linked-list, no longer needed

	// Receive message from server
	if((bytesReceived = recv(sockfd, buffer, MAXDATASIZE - 1, 0)) == -1)
	{
		fprintf(stderr, "Client: receive error");
		exit(EXIT_FAILURE);
	}

	buffer[bytesReceived] = '\0';

	printf("Client: received '%s'\n", buffer);

	// Shutdown client
	close(sockfd);

	return 0;

}

/* Print command line argument format */
void printUsage()
{ 
	printf("Usage ./chatclient <server-hostname> <port#>\n");
}

