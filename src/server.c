
#include "server.h"


struct Server* create_server(char* address, uint16_t port, uint32_t max_connections) {
    struct Server* server = calloc(1, sizeof(struct Server));
    if (server == NULL) {
        fprintf(stderr, "Error allocating memory for server");
        exit(EXIT_FAILURE);
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        fprintf(stderr, "Error creating socket");
        exit(EXIT_FAILURE);
    }

    server->socket_fd = socket_fd;
    server->config.sin_family = AF_INET;
    server->config.sin_port = htons(port);
    server->max_connections = max_connections;

    if (bind(socket_fd, (struct sockaddr*) &(server->config), sizeof(server->config)) == -1) {
        fprintf(stderr, "Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server bind to %s:%d\n", address, port);

    return server;
}

void start_server(struct Server* server) {
    if (listen(server->socket_fd, server->max_connections) == -1) {
        fprintf(stderr, "Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", ntohs(server->config.sin_port));

    while (1) {
        int client_fd = accept(server->socket_fd, NULL, NULL);
        if (client_fd == -1) {
            fprintf(stderr, "Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        char buffer[1024];
        int bytes_read = read(client_fd, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            fprintf(stderr, "Read failed");
            exit(EXIT_FAILURE);
        }

        printf("Received %d bytes\n", bytes_read);

        int bytes_written = write(client_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            fprintf(stderr, "Write failed");
            exit(EXIT_FAILURE);
        }

        printf("Sent %d bytes\n", bytes_written);

        close(client_fd);
        printf("Client disconnected\n");
    }
}

void stop_server(struct Server* server) {
    if (shutdown(server->socket_fd, SHUT_RDWR) < 0) {
        fprintf(stderr, "Shutdown failed");
        exit(EXIT_FAILURE);
    }
}

void delete_server(struct Server* server) {
    stop_server(server);
    free(server);
}