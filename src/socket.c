#include "sys/socket.h"
#include <arpa/inet.h>
#include <bits/types/cookie_io_functions_t.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "errno.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int is_ip_address(const char *hostname) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, hostname, &(sa.sin_addr)) == 1;
}

int socketCreateConnect(const char *hostname,const char *port){
    struct sockaddr_in server_addr;
    int sockfd;

    if (is_ip_address(hostname)) {
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(port));
        if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
            perror("Invalid ip address");
            exit(EXIT_FAILURE);
        }
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
         int socketconnect = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
            
            if(socketconnect < 0){
                close(sockfd);
                perror("socket Connection failed");
                exit(EXIT_FAILURE);
            }
    } else {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(hostname, port, &hints, &res) != 0) {
            perror("getaddrinfo failed");
            exit(EXIT_FAILURE);
        }
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
            close(sockfd);
            freeaddrinfo(res);
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }
        freeaddrinfo(res);
    }
    return sockfd ;
}
void httpRequest(int socketfd, const char *method, const char *hostname,const char *path, const char *data){
      char request[BUFFER_SIZE];
    if (strcasecmp(method, "POST") == 0) {
        snprintf(request, sizeof(request),
                 "POST %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Content-Type: application/x-www-form-urlencoded\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: close\r\n\r\n"
                 "%s",
                 path, hostname, strlen(data), data);
    } else if (strcasecmp(method, "GET") == 0) {
        snprintf(request, sizeof(request),
                 "GET %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Connection: close\r\n\r\n",
                 path, hostname);
    }
    int sendRequest = send(socketfd,request, strlen(request),0);
}

void recvRequest(int sockfd){
    char response[BUFFER_SIZE];
    ssize_t recievedResponse;
    recievedResponse = recv(sockfd,response,sizeof(response) - 1,0);
    if(recievedResponse < 0){
        perror("recv failed!");
        exit(EXIT_FAILURE);
    }
    response[recievedResponse] = '\0';
    printf("%s",response);
}
