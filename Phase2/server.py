import socket
import threading

from middle_ware import handle_client

class Server:
    def __init__(self, host, port, max_connections):

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((host, port))
        self.server_socket.listen(max_connections)
        self.client_threads = []
    
    def run(self):    
        
        while True:
            client_fd, client_addr = self.server_socket.accept()
            print(f"Connection from {client_addr}")
            client_thread = threading.Thread(target=handle_client, args=(client_fd,))
            client_thread.start()
            self.client_threads.append(client_thread)

    def stop(self):
        for client_thread in self.client_threads:
            client_thread.join()
        self.server_socket.close()
