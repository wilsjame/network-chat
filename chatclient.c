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
#include <stdbool.h>

#define BACKLOG 1 // Pending connections queue will hold
#define MAXDATASIZE 500
#define HANDLESIZE 420

void printUsage();
bool quitIn(char* buffer);
void getHandle(char* handle);

int main(int argc, char*argv[])
{
	
	// Check command line bad usage.
	// Only checks if the number of arguments is wrong.
	if(argc != 3)
	{
		printUsage();
		exit(1);
	}

	char handle[HANDLESIZE];
	getHandle(handle);
	
	struct addrinfo hints;			// Fill out with relevent info
	struct addrinfo *result, *rp;		// Will point to results
	int sockfd, status, bytesReceived;
	int yes = 1;
	char buffer[MAXDATASIZE];
	char initialMessage[] = "Client connection established.";

	memset(&hints, 0, sizeof(hints));	// Make sure the struct is empty
	hints.ai_family = AF_UNSPEC;		// Don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets (woot!)
	hints.ai_flags = AI_PASSIVE;		// Fill in my IP for me
	hints.ai_protocol = 0;			// Any protocol
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	// *** Set up socket ***
	
	// Set up the address struct for this process (the client)
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

		// Create socket
		if((sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) 
		{
			fprintf(stderr, "Client: socket error\n");
		}

		// Forcefully attach socket to port
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(int)) == -1)
		{
			fprintf(stderr, "Client: setsockopt error\n");
		}

		if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) 
		{
			printf("Server connection established.\n");
			break; // Success!
		}
		else
		{
			fprintf(stderr, "Client: create socket error\n");
		}

		// close bad socket and try again
		close(sockfd);
	}

	// No address succeeded.
	if(rp == NULL) 
	{
		fprintf(stderr, "Client: failed to create socket\n");
		exit(EXIT_FAILURE);
	}

	// Free the linked-list, no longer needed
	freeaddrinfo(result); 

	// *** Chat ***
	
	
	// Send initial message to establish a connecion.
	send(sockfd, initialMessage, strlen(initialMessage), 0);

	// Chat with server.
	while(true)
	{

		// Receive data from server in max-sized byte packets
		if((bytesReceived = recv(sockfd, buffer, MAXDATASIZE - 1, 0)) == -1)
		{
			fprintf(stderr, "Client: receive error");
			exit(EXIT_FAILURE);
		}

		buffer[bytesReceived] = '\0';

		// Parse server response for '/quit' keyword
		if(bytesReceived > 5 && quitIn(buffer))
		{
			printf("Server closed their connection\n");
			close(sockfd);
		}
		else
		{
			// Send a reply here..?
			printf("'%s'\n", buffer);
		}

		// Send a reply or here..?

	}

	return 0;

}

// Print command line argument format
void printUsage()
{ 
	printf("Usage ./chatclient <server-hostname> <port#>\n");
}

// Parse buffer for quit keyword
bool quitIn(char* buffer)
{

	int itr = 0;

	while(buffer[itr] != '\0')
	{

		if(buffer[itr++] == '/')
		{

			if(buffer[itr++] == 'q')
			{

				if(buffer[itr++] == 'u')
				{

					if(buffer[itr++] == 'i')
					{

						if(buffer[itr] == 't')
						{

							return true;

						}

					}

				}

			}

		}

	}

	return false;

}

// Gets user input and stores it in handle.
void getHandle(char* handle)
{

	// First things first, clear handle string.
	memset(handle, '\0', HANDLESIZE);

	// Magic settings. 
	size_t bufferSize = 0;
	char* lineEntered = NULL;
	
	// Display prompt and then flush all open output steams. 
	printf("Enter your handle: ");
	fflush(NULL);

	// By magic settings, getline() buffers automatically use malloc. 
	getline(&lineEntered, &bufferSize, stdin);

	// Remove trailing '\n' from pressing enter. 
	lineEntered[strcspn(lineEntered, "\n")] = 0;

	// Store user input for use in main().
	strcpy(handle, lineEntered);

	// Give it a carrot.
	strcat(handle, ">");

	// Free memory allocated by getline() (free Lil B).
	free(lineEntered);

	return;

}

