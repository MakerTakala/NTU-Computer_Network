#include "http.h"


char* respond_http_to_string(RespondHttp* respond_http) {
    char* respond_http_string = (char*)calloc(sizeof(char), BUFFER_SIZE);
    sprintf(respond_http_string, "%s %s %s\r\n\n", respond_http->protocol, respond_http->status_code, respond_http->status_text);
    HttpHeader* header = respond_http->headers;
    while (header != NULL) {
        sprintf(respond_http_string, "%s%s: %s\r\n", respond_http_string, header->key, header->value);
        header = header->next;
    }
    return respond_http_string;
}

char* request_http_to_string(RequestHttp* request_http) {
    char* request_http_string = (char*)calloc(sizeof(char), BUFFER_SIZE);
    sprintf(request_http_string, "%s %s %s\r\n\n", request_http->method, request_http->path, request_http->protocol);
    HttpHeader* header = request_http->headers;
    while (header != NULL) {
        sprintf(request_http_string, "%s%s: %s\r\n", request_http_string, header->key, header->value);
        header = header->next;
    }
    return request_http_string;
}

RespondHttp* respond_http_from_string(char* respond_http_string) {
    RespondHttp* respond_http = (RespondHttp*)malloc(sizeof(RespondHttp));
    char* token = strtok(respond_http_string, " ");
    respond_http->protocol = (char*)malloc(sizeof(char) * strlen(token));
    strcpy(respond_http->protocol, token);
    token = strtok(NULL, " ");
    respond_http->status_code = (char*)malloc(sizeof(char) * strlen(token));
    strcpy(respond_http->status_code, token);
    token = strtok(NULL, " ");
    respond_http->status_text = (char*)malloc(sizeof(char) * strlen(token));
    strcpy(respond_http->status_text, token);
    token = strtok(NULL, "\r\n");
    while (token != NULL) {
        HttpHeader* header = (HttpHeader*)malloc(sizeof(HttpHeader));
        header->key = (char*)malloc(sizeof(char) * strlen(token));
        strcpy(header->key, token);
        token = strtok(NULL, "\r\n");
        header->value = (char*)malloc(sizeof(char) * strlen(token));
        strcpy(header->value, token);
        token = strtok(NULL, "\r\n");
        header->next = respond_http->headers;
        respond_http->headers = header;
    }
    return respond_http;
}

RequestHttp* request_http_from_string(char* request_http_string) {
    RequestHttp* request_http = (RequestHttp*)malloc(sizeof(RequestHttp));
    char* token = strtok(request_http_string, " ");
    request_http->method = (char*)malloc(sizeof(char) * strlen(token));
    strcpy(request_http->method, token);
    token = strtok(NULL, " ");
    request_http->path = (char*)malloc(sizeof(char) * strlen(token));
    strcpy(request_http->path, token);
    token = strtok(NULL, " ");
    request_http->protocol = (char*)malloc(sizeof(char) * strlen(token));
    strcpy(request_http->protocol, token);
    token = strtok(NULL, "\r\n");
    while (token != NULL) {
        HttpHeader* header = (HttpHeader*)malloc(sizeof(HttpHeader));
        header->key = (char*)malloc(sizeof(char) * strlen(token));
        strcpy(header->key, token);
        token = strtok(NULL, "\r\n");
        header->value = (char*)malloc(sizeof(char) * strlen(token));
        strcpy(header->value, token);
        token = strtok(NULL, "\r\n");
        header->next = request_http->headers;
        request_http->headers = header;
    }
    return request_http;
}