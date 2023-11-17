# write a client program that will connect to the server

import socket

if __name__ == '__main__':
    
    # create a socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # get local machine name
    host = socket.gethostname()

    port = 8080

    # connection to hostname on the port.
    s.connect((host, port))

    # Receive no more than 1024 bytes
    msg = s.recv(1024)

    s.close()

    print(msg.decode('ascii'))
