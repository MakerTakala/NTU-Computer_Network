#include "contect.h"

static char* read_file(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "File not found: %s\n", path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    int32_t file_size = ftell(file);
    rewind(file);

    char *buffer = calloc(1, file_size + 1);
    fread(buffer, file_size, 1, file);
    fclose(file);

    buffer[file_size] = '\0';
    return buffer;
}

static char* get_profile() {
    RespondHttp *respond = calloc(1, sizeof(RespondHttp));
    respond->protocol = "HTTP/1.1";
    respond->status_code = "200";
    respond->status_text = "OK";
    
    HttpHeader *content_type = calloc(1, sizeof(HttpHeader));
    content_type->key = "Content-Type";
    content_type->value = "text/html";
    content_type->next = NULL;

    respond->headers = content_type;

    char *profile = read_file("./src/web/index.html");
    respond->body = profile;

    char *respond_string = respond_http_to_string(respond);
    return respond_string;
}

static char* post_reflect(char *message) {
    return message;
}

int32_t contect(int32_t client_fd) {
    char *read_buffer = calloc(BUFFER_SIZE, sizeof(char));
    if (recv(client_fd, read_buffer, BUFFER_SIZE, 0) == -1) {
        fprintf(stderr, "Recv failed");
        return -1;
    }
    if (strlen(read_buffer) == 0) {
        return 0;
    }

    printf("Received:\n%s\n", read_buffer);
    RequestHttp *request = request_http_from_string(read_buffer);
    free(read_buffer);
    char *send_buffer = NULL;
    if (request == NULL) {
        fprintf(stderr, "Request is Wrong");
        return -1;
    }
    else if (!strcmp(request->method, "GET")) {
        if (!strcmp(request->path, "/")) {
            send_buffer = get_profile();
        }
    }
    else if (!strcmp(request->method, "POST")) {
        if (!strcmp(request->path, "/")) {
            send_buffer = post_reflect(request->body);
        }   
    }
    else {
        fprintf(stderr, "Method not supported");
        return -1;
    }
    free(request);
    
    printf("Sending:\n%s\n", send_buffer);

    if (send(client_fd, send_buffer, BUFFER_SIZE, 0) == -1) {
        printf("client closed connection.\n");
        free(send_buffer);
        close(client_fd);
        return 0;
    }
    free(send_buffer);
    close(client_fd);
    return 1;
}