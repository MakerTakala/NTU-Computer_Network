#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../const.h"

int main()
{
    int sock_fd;
    struct sockaddr_in serv_name;
    int status;
    

    // create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // server address
    serv_name.sin_family = AF_INET;
    inet_aton(HOST, &serv_name.sin_addr);
    serv_name.sin_port = htons(PORT);

    status = connect(sock_fd, (struct sockaddr *)&serv_name, sizeof(serv_name));
    if (status == -1) {
        perror("Connection error");
        exit(1);
    }

    while (1) {
        char *message = calloc(BUFFER_SIZE, sizeof(char));
        char *read_buffer = calloc(BUFFER_SIZE, sizeof(char)), *send_buffer = calloc(BUFFER_SIZE, sizeof(char));
        printf("please input message: ");
        fgets(message, BUFFER_SIZE, stdin);

        if(!strcmp(message, "exit\n")) {
            close(sock_fd);
            printf("EXIT.\n");
            exit(EXIT_SUCCESS);
        }

        sprintf(send_buffer, "POST / HTTP/1.1\r\nHost: localhost:%d\r\n\r\n", PORT);
        sprintf(send_buffer, "%s%s", send_buffer, message);
        free(message);
        printf("Sending:\n%s", send_buffer);

        send(sock_fd, send_buffer, BUFFER_SIZE, 0);
        free(send_buffer);
        
        if (recv(sock_fd, read_buffer, BUFFER_SIZE, 0) == -1) {
            close(sock_fd);
            printf("server closed connection.\n");
            perror("Recv failed");
            break;
        }
        printf("\nReceived:\n%s\n", read_buffer);
        free(read_buffer);  
    }

    return 0;
}