// http_utils.c
#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096

void errorExit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

void parseUrl(const char *url, char *hostname, char *path) {
    const char *slash = strchr(url, '/');
    if (slash) {
        strncpy(hostname, url, slash - url);
        hostname[slash - url] = '\0';
        strcpy(path, slash);
    } else {
        strcpy(hostname, url);
        strcpy(path, "/");
    }
}

int isIpAddress(const char *hostname) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, hostname, &(sa.sin_addr)) == 1;
}

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        errorExit("Socket creation failed");
    }
    return sockfd;
}

void connectSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (connect(sockfd, addr, addrlen) < 0) {
        close(sockfd);
        errorExit("Connection failed");
    }
}

void sendRequest(int sockfd, const char *request) {
    if (send(sockfd, request, strlen(request), 0) < 0) {
        close(sockfd);
        errorExit("Send failed");
    }
}

void receiveResponse(int sockfd) {
    char response[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        printf("%s", response);
    }

    if (bytes_received < 0) {
        errorExit("Receive failed");
    }
}

void receiveHttpRequest(const char *method, const char *hostname, const char *path, const char *data, char *request) {
    if (strcasecmp(method, "POST") == 0) {
        snprintf(request, BUFFER_SIZE,
                 "POST %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Content-Type: application/x-www-form-urlencoded\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: close\r\n\r\n"
                 "%s",
                 path, hostname, strlen(data), data);
    } else if (strcasecmp(method, "GET") == 0) {
        snprintf(request, BUFFER_SIZE,
                 "GET %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Connection: close\r\n\r\n",
                 path, hostname);
    }
}

