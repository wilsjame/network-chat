INSTRUCTIONS:

1) Run chatserver
	$ chmod +x chatserve
	$ ./chatserve <port#>

	or

	$ python3 chatserve <port#>

2) Compile chatclient.c
	$ make

3) Run chatclient
	$ ./chatclient <server-hostname> <port#>

4) On the client, enter your handle. 
   An initial message will automatically be 
   sent to the server to establish a connection. 

5) On the server, enter a message and press enter.

6) On the client, enter a message and press enter. 

7) Repeat steps 5 and 6 to continue chat.

8) Continue chatting until '\quit' is entered on 
   either side. The client will terminate and the
   server will resume listening on port# for a 
   new client connection. 

9) Terminate the server by sending it SIGINT with
   CTRL+C. 


CITED SOURCES:

I couldn't have made this without the aid from
these sources, check them out!

- https://beej.us/guide/bgnet/
- https://docs.python.org/3/library/socket.html
- https://wiki.python.org/moin/TcpCommunication
- gettaddrinfo(3) man page example


NOTES:

This program was tested on Oregon State University's
Linux engineering server: flip1.engr.oregonstate.edu

