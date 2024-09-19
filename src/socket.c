#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

char *argv_parse;

int sockfd;
struct sockaddr_in server_addr;
struct hostent *server;
char buffer[4096];


int create_socket() {
    // Step 1: Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int server_connect(char *hostname) {
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected to Server!\n");
    return 0;
}
int server_struct() {
    // Step 4: Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);  // HTTP port 80
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    return 0;
}

int server_connect2() {
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }else
        printf("Connection established!\n");


}
int send_request(char *hostname){
    char request[1024];
    snprintf(request, sizeof(request), "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", hostname);
    send(sockfd, request, strlen(request), 0);
    return 0;
}

int receive_socket() {

    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the buffer
        printf("%s", buffer);  // Print the response
    }

    if (bytes_received < 0) {
        perror("Receiving failed");
    }
    return 0;
}
