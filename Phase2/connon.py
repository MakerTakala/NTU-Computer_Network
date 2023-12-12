# ping 0.0.0.0:8080 with mulitple connections with client

import socket
import threading


HOST = "0.0.0.0"
PORT = 8080

def connect():
    while True:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
        request = "GET / HTTP/1.1\r\n\r\n"
        client_socket.send(str.encode(request))
        response = client_socket.recv(1024).decode()
        print(response)
        client_socket.close()
    

if __name__ == '__main__':
   
    
    for i in range(50):
        client_thread = threading.Thread(target=connect)
        client_thread.start()
        client_thread.join()
        
    print("Done!")

