#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096
void send_http_request(int sockfd, const char *ip,const char *host, const char *method, const char *path) {
    struct sockaddr_in server_addr;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char post_data[] = "key1=value1&key2=value2"; 
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connection secured!\n");

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
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    send(sockfd, request, strlen(request), 0);

    printf("Response:\n");
    while (recv(sockfd, response, sizeof(response) - 1, 0) > 0) {
        printf("%s", response);
        memset(response, 0, sizeof(response));
    }

}
