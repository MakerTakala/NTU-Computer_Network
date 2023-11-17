import socket
import threading

from middle_ware import handle_client

class Server:
    def __init__(self, host, port, max_connections):
        self.host = host
        self.port = port
        self.max_connections = max_connections
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(self.max_connections)
    
    def run(self):       
        while True:
            client_fd, client_addr = self.server_socket.accept()
            print(f"Connection from {client_addr}")
            client_thread = threading.Thread(target=handle_client, args=(client_fd,))
            client_thread.start()
            

    def stop(self):
        self.server_socket.close()
