#/*********************************************************************
#** Author: James G Wilson
#** Date: 5/4/2018
#** Description: chatserve.py 
#*********************************************************************/

# Access socket methods without having to prepend 'socket.'
from socket import * 

def main():

	# Hard code server's handle.
	handle = 'Host A (server)'
	# print(handle)

	while True:

		# create IPv6 TCP listening socket
		# bind port # with listening socket
		serverPort = input('Enter server port#: ')
		serverSocket = socket(AF_INET6, SOCK_STREAM)
		serverSocket.bind(('', int(serverPort)))

		# start listening with backlog = 1
		serverSocket.listen(1)
		print('Server is now listening and ready to receive...')

		# wait for client to come knocking
		connectionSocket, addr = serverSocket.accept()

		# Establish connection with client.
		clientMessage = connectionSocket.recv(1024);
		print(clientMessage.decode())
		chat = True

		# Begin chatting
		while chat:

			# Send
			message = input('>>')

			if '\quit' in message:
				chat = False

			connectionSocket.send(message.encode())
			
			# Receive
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


