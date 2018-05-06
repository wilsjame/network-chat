#/*********************************************************************
#** Author: James G Wilson
#** Date: 5/6/2018
#** Description: chatserve.py 
#*********************************************************************/

# Access socket methods without having to prepend 'socket.'
from socket import * 

def main():

	# Hard code server's handle.
	handle = 'Server> '

	# Keep server alive until SIGINT received.
	while True:

		# Create IPv6 TCP listening socket.
		# Bind port # with listening socket.
		serverPort = input('Enter server port#: ')
		serverSocket = socket(AF_INET6, SOCK_STREAM)
		serverSocket.bind(('', int(serverPort)))

		# Start listening with backlog = 1.
		serverSocket.listen(1)
		print('Server is now listening and ready to receive...')

		# Wait for client to come knocking.
		connectionSocket, addr = serverSocket.accept()

		# Establish connection with client.
		clientMessage = connectionSocket.recv(1024);
		print(clientMessage.decode())
		chat = True

		while chat:

			# Send message.
			message = handle + input('>> ')

			if '\quit' in message:
				chat = False

			connectionSocket.send(message.encode())
			
			# Receive message.
			if chat:
				clientMessage = connectionSocket.recv(1024);
				print(clientMessage.decode())

				if '\quit' in clientMessage.decode():
					print('Client closed their connection.')
					chat = False

			if chat == False:
				connectionSocket.close()

if __name__ == "__main__":

	# execute only if run as a script
	main()

