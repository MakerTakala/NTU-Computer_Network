#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        char indata[BUFFER_SIZE] = {0}, outdata[BUFFER_SIZE] = {0};
        printf("please input message: ");
        fgets(outdata, sizeof(outdata), stdin);

        if(!strcmp(outdata, "exit\n")) {
            close(sock_fd);
            printf("EXIT.\n");
            exit(EXIT_SUCCESS);
        }

        printf("Sending: %s\n", outdata);
        send(sock_fd, outdata, strlen(outdata), 0);

        int nbytes = recv(sock_fd, indata, sizeof(indata), 0);

        if (nbytes <= 0) {
            close(sock_fd);
            printf("server closed connection.\n");
            break;
        }
        printf("Received: %s\n", indata);
    }

    return 0;
}