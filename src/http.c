#include "http.h"


char* respond_http_to_string(RespondHttp *respond_http) {
    char *respond_http_string = (char*)calloc(BUFFER_SIZE, sizeof(char));
    sprintf(respond_http_string, "%s %s %s\r\n", respond_http->protocol, respond_http->status_code, respond_http->status_text);
    HttpHeader *header = respond_http->headers;
    while (header != NULL) {
        sprintf(respond_http_string, "%s%s: %s\r\n", respond_http_string, header->key, header->value);
        header = header->next;
    }
    if (respond_http->body != NULL) {
        sprintf(respond_http_string, "%s\r\n%s", respond_http_string, respond_http->body);
    }
    return respond_http_string;
}

char* request_http_to_string(RequestHttp *request_http) {
    char *request_http_string = (char*)calloc(BUFFER_SIZE, sizeof(char));
    sprintf(request_http_string, "%s %s %s\r\n", request_http->method, request_http->path, request_http->protocol);
    HttpHeader *header = request_http->headers;
    while (header != NULL) {
        sprintf(request_http_string, "%s%s: %s\r\n", request_http_string, header->key, header->value);
        header = header->next;
    }
    if (request_http->body != NULL) {
        sprintf(request_http_string, "%s\r\n%s", request_http_string, request_http->body);
    }
    sprintf(request_http_string, "%s\r\n%s", request_http_string, request_http->body);
    return request_http_string;
}

RespondHttp* respond_http_from_string(char *respond_http_string) {
    if (respond_http_string == NULL) {
        return NULL;
    }
    
    char *header_body_spilt = strstr(respond_http_string, "\r\n\r\n");
    char *header_token = respond_http_string;
    char *body_token = NULL;

    if (header_body_spilt != NULL) {
        for (size_t i = 0; i < 4; i++) {
            header_body_spilt[i] = '\0';
        }
        body_token = header_body_spilt + 4;
    }

    RespondHttp *respond_http = (RespondHttp*)calloc(1, sizeof(RespondHttp));

    char *token = strtok(header_token, " ");
    if (token == NULL) return NULL;
    respond_http->protocol = (char*)calloc(strlen(token), sizeof(char));
    strcpy(respond_http->protocol, token);

    token = strtok(NULL, " ");
    if (token == NULL) return NULL;
    respond_http->status_code = (char*)calloc(strlen(token), sizeof(char));
    strcpy(respond_http->status_code, token);

    token = strtok(NULL, "\r\n");
    if (token == NULL) return NULL;
    respond_http->status_text = (char*)calloc(strlen(token), sizeof(char));
    strcpy(respond_http->status_text, token);

    while (1) {
        token = strtok(NULL, "\n");
        if (token == NULL) {
            break;
        }
        HttpHeader *header = (HttpHeader*)calloc(1, sizeof(HttpHeader));

        char *split = strstr(token, ": ");
        if (split == NULL) return NULL;
        split[0] = '\0';

        header->key = (char*)calloc(strlen(token), sizeof(char));
        strcpy(header->key, token);

        token += strlen(token) + 2;
        header->value = (char*)calloc(strlen(token), sizeof(char));
        strcpy(header->value, token);
        
        header->next = respond_http->headers;
        respond_http->headers = header;
    }

    if (body_token != NULL) {
        respond_http->body = (char*)calloc(strlen(body_token), sizeof(char));
        strcpy(respond_http->body, body_token);
    }
    
    return respond_http;
}

RequestHttp* request_http_from_string(char *request_http_string) {
    if (request_http_string == NULL) {
        return NULL;
    }
    
    char *header_body_spilt = strstr(request_http_string, "\r\n\r\n");
    char *header_token = request_http_string;
    char *body_token = NULL;

    if (header_body_spilt != NULL) {
        for (size_t i = 0; i < 4; i++) {
            header_body_spilt[i] = '\0';
        }
        body_token = header_body_spilt + 4;
    }

    RequestHttp *request_http = (RequestHttp*)calloc(1, sizeof(RequestHttp));

    char *token = strtok(header_token, " ");
    if (token == NULL) return NULL;
    request_http->method = (char*)calloc(strlen(token), sizeof(char));
    strcpy(request_http->method, token);

    token = strtok(NULL, " ");
    if (token == NULL) return NULL;
    request_http->path = (char*)calloc(strlen(token), sizeof(char));
    strcpy(request_http->path, token);

    token = strtok(NULL, "\r\n");
    if (token == NULL) return NULL;
    request_http->protocol = (char*)calloc(strlen(token), sizeof(char));
    strcpy(request_http->protocol, token);

    while (1) {
        token = strtok(NULL, "\n");
        if (token == NULL) {
            break;
        }
        HttpHeader *header = (HttpHeader*)calloc(1, sizeof(HttpHeader));

        char *split = strstr(token, ": ");
        if (split == NULL) return NULL;
        split[0] = '\0';

        header->key = (char*)calloc(strlen(token), sizeof(char));
        strcpy(header->key, token);

        token += strlen(token) + 2;
        header->value = (char*)calloc(strlen(token), sizeof(char));
        strcpy(header->value, token);
        
        header->next = request_http->headers;
        request_http->headers = header;
    }

    if (body_token != NULL) {
        request_http->body = (char*)calloc(strlen(body_token), sizeof(char));
        strcpy(request_http->body, body_token);
    }
    
    return request_http;
}