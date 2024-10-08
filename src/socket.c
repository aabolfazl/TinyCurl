#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>

#define BUFFER_SIZE 4096

// Function to send a GET or POST request using plain sockets (HTTP)
void send_http_request(const char *host, const char *method, const char *path, const char *data) {
    struct addrinfo hints, *res;
    int sockfd;
    char request[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    // Set up the address info hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Resolve the host name to an address
    int status = getaddrinfo(host, "80", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Create the socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    // Create the HTTP request based on the method (GET or POST)
    if (strcmp(method, "GET") == 0) {
        snprintf(request, sizeof(request),
                 "GET %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Connection: close\r\n\r\n",
                 path, host);
    } else if (strcmp(method, "POST") == 0) {
        snprintf(request, sizeof(request),
                 "POST %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Content-Type: application/x-www-form-urlencoded\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n\r\n"
                 "%s",
                 path, host, strlen(data), data);
    } else {
        fprintf(stderr, "Unsupported method: %s\n", method);
        exit(1);
    }

    // Send the request to the server
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        exit(1);
    }

    // Receive the response from the server
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer); // Print the server's response
    }

    if (bytes_received == -1) {
        perror("recv");
    }

    // Clean up
    freeaddrinfo(res);
    close(sockfd);
}

int socket_create(const char *host, int port) {
    struct addrinfo hints, *result;
    int socket_fd;
    char port_str[6];

    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(host, port_str, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return status;
    }

    socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socket_fd == -1) {
        freeaddrinfo(result);
        return socket_fd;
    }

    freeaddrinfo(result);

    return socket_fd;
}