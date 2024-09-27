#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80

int sock;
struct sockaddr_in server;
char message[1024], server_reply[4096];

void socket_create() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed!");
        perror(EXIT_SUCCESS);
    }
    printf("Socket created\n");
}

int socket_connect(const char *url) {
    server.sin_addr.s_addr = inet_addr(url);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    printf("Binding to socket...\n");
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Connection established\n");
    return 0;
}

int socket_getPost(char *url, char *request_type, char *path) {
    //Argument Parse check
    if (url != NULL && request_type != NULL || path != NULL) {
        return 0;
    }

    if (strcmp(request_type, "post") == 0) {
        // Prepare and send POST request
        char *data = "field1=value1&field2=value2";
        snprintf(message, sizeof(message),
                 "POST %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Content-Type: application/x-www-form-urlencoded\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n\r\n"
                 "%s", path, url, strlen(data), data);
    } else if (strcmp(request_type, "get") == 0) {
        // Prepare and send GET request
        snprintf(message, sizeof(message),
                 "GET %s HTTP/1.1\r\n"
                 "Host: %s\r\n"
                 "Connection: close\r\n\r\n", path, url);
    } else {
        printf("Invalid request type. Use 'get' or 'post'.\n");
        close(sock);
        return 1;
    }
    return 0;
}

int socket_send() {
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Send failed");
        return 1;
    }
    return 0;
}

int socket_recv() {
    int recv_size;
    while ((recv_size = recv(sock, server_reply, sizeof(server_reply) - 1, 0)) > 0) {
        server_reply[recv_size] = '\0'; // Null-terminate the server's reply
        printf("%s", server_reply); // Print the reply to the terminal
    }

    if (recv_size < 0) {
        perror("Receive failed");
    }
    return 0;
}

void socket_close() {
    close(sock);
}
