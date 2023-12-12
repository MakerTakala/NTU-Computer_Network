import signal

from server import Server


HOST = "0.0.0.0"
PORT = 8080
MAX_CONNECTIONS = 20

server = Server(HOST, PORT, MAX_CONNECTIONS)

def handler(signum, frame):
    server.stop()
    print("Server stopped!")


if __name__ == '__main__':
    signal.signal(signal.SIGINT, handler)
    server.run()
    
