# Reference: Used python code from lecture and from https://www.digitalocean.com/community/tutorials/python-socket-programming-server-client

# CLIENT

import socket

#create a socket object
host = socket.gethostname()
#define the port on which you want to connect
port = 9876

s = socket.socket()
#connect to the server on local computer
s.connect((host, port))

message = input(" -> ")  # take input

while message.lower().strip() != 'client':
    s.send(message.encode())
    #receive data from the server
    data = c.recv(1024).decode()
    print('Received from server: ' + data)

    message = input(" -> ")  # again take input
# close the connection
s.close()

