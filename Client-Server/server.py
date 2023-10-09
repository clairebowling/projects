# Reference: Used python code from lecture and from https://www.digitalocean.com/community/tutorials/python-socket-programming-server-client

# SERVER

#setup 
import socket
import random

s = socket.socket()
print("Socketsuccessfullycreated")

#reserve a port on your computer
port = 9876

#next bind to the port
s.bind(('',port))
print ("socket binded to %s" %(port))

#put the socket into listening mode
s.listen(5)
print("socket is listening")

#loop forever until we interrupt it or an error occurs
#with open("asg5DataFile.txt") as f:
#    data_list = f.readlines()
c, addr = s.accept()
print("Got connection from ",addr)
while True:
#establish connection with client.
    data = c.recv(1024).decode()
    if not data:
        # if data is not received break
        break
    print("from connected user: " + str(data))
    data = input(' -> ')
    #send a thank you message to the client.
    c.send("You are awesome")
    #close the connection
c.close()
