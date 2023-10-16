#include "contect.h"


void contect(int32_t client_fd) {
    while(1) {
        char read_buffer[BUFFER_SIZE] = {0};
        if (recv(client_fd, read_buffer, sizeof(read_buffer), 0) == -1) {
            fprintf(stderr, "Recv failed");
            close(client_fd);
            break;
        }
        if (strlen(read_buffer) == 0) {
            printf("Client disconnected\n");
            close(client_fd);
            break;
        }

        printf("Received: %s\n", read_buffer);

        char send_buffer[BUFFER_SIZE] = "HTTP/1.1 200 OK\n\
Content-Type: text/html\n\n\
<!DOCTYPE html>\n\
<html>\n\
<head>\n\
    <title>Takala</title>\n\
</head>\n\
<body>\n\
    <h1>My name is Takala.</h1>\n\
    <h2>41047025S</h2>\n\
</body>\n\
</html>\n\
";

        printf("Sending: %s\n", send_buffer);

        if (send(client_fd, send_buffer, sizeof(send_buffer), 0) == -1) {
            fprintf(stderr, "Send failed");
            close(client_fd);
            break;
        }
    }
}