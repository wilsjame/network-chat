#/*********************************************************************
#** Author: James G Wilson
#** Date: 5/4/2018
#** Description: chatserve.py 
#*********************************************************************/

# access socket methods without having to prepend 'socket.'
from socket import * 

def main():

	# hard code server's handle
	handle = 'Host A (server)'
	print(handle)

	# get port # from command line and convert to int
	serverPort = input('Enter server port #: ')
	print('You entered ' + serverPort)

	# create IPv6 TCP listening socket
	serverSocket = socket(AF_INET6, SOCK_STREAM)

	# bind port # with listening socket
	serverSocket.bind(('', int(serverPort)))

	# start listening with backlog = 1
	serverSocket.listen(1)
	print('Server is now listening and ready to receive...')

	# wait, indefinitely, for client to come knocking
	while True:

		# addr is address bound to the (client's)
		# socket on the other end of the connection
		connectionSocket, addr = serverSocket.accept()

		# encode string into bytes and send
		message = 'Hello from ' + handle + '!'
		connectionSocket.send(message.encode())

	# give incoming client their own connection socket
	# chat!
	#	prepend handle
	#	respond to host B
	#	alternate
	#	close connection with command "\quit"
	# close connection socket
	# repeat 
	# signal handler: catch SIGINT to shutdown server

if __name__ == "__main__":
	# execute only if run as a script
	main()


