#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096

int create_socket(const char *ip) { 
    int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0) {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created!\n");  
    return socketFileDescriptor;
}

void socket_connect(int socketFileDescriptor,const char *ip){
    struct sockaddr_in server_addr; 

    server_addr.sin_family = AF_INET ;
    server_addr.sin_port = htons(80);
    inet_pton(AF_INET,ip , &server_addr.sin_addr);
     
    if (connect(socketFileDescriptor, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(socketFileDescriptor);
        exit(EXIT_FAILURE);
    }
    printf("Connection secured!\n");
}

void socket_send_recv(int socketFileDescriptor, const char *host, const char *method, const char *path) {
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char post_data[] = "key1=value1&key2=value2";
    int total_received = 0;
    
    if (strcmp(method, "GET") == 0) {
        snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    } else if (strcmp(method, "POST") == 0) {
        snprintf(request, sizeof(request),
            "POST %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: %ld\r\n\r\n"
            "%s",
            path, host, strlen(post_data), post_data);
    } else {
        fprintf(stderr, "Unsupported method: %s\n", method);
        close(socketFileDescriptor);
        exit(EXIT_FAILURE);
    }

    int sendRequest = send(socketFileDescriptor, request, strlen(request), 0);
    if(sendRequest < 0) {
        printf("Send was unsuccessful!\n");
        close(socketFileDescriptor);
        exit(EXIT_FAILURE);
    }
    printf("Send was successful!\n");

    printf("Response:\n");

    int received, content_length = -1;
    char *content_ptr = NULL;
    while ((received = recv(socketFileDescriptor, response + total_received, sizeof(response) - total_received - 1, 0)) > 0) {
        total_received += received;
        response[total_received] = '\0'; 
        
        if (content_length == -1) {
            char *content_length_str = strstr(response, "Content-Length:");
            if (content_length_str) {
                sscanf(content_length_str, "Content-Length: %d", &content_length);
                content_ptr = strstr(response, "\r\n\r\n") + 4;  
                if (content_ptr) {
                    int header_size = content_ptr - response;
                    int body_size = total_received - header_size;
                    if (body_size >= content_length) break;
                }
            }
        } else {
            if (total_received - (content_ptr - response) >= content_length) {
                break; 
            }
        }
    }

    if (received < 0) {
        printf("Receive was unsuccessful!\n");
        close(socketFileDescriptor);
        exit(EXIT_FAILURE);
    } else {
        printf("%s", response);
    }
    
    close(socketFileDescriptor);
}

