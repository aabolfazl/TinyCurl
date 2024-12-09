#include "sys/socket.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "errno.h"
#include <string.h>

#define BUFFER_SIZE 4096

int createSocket(){
    int socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd < 0){
        perror("Error for creating socket");
        EXIT_FAILURE;
    }
    return socketfd;
}

void socketConnect(int socket,const char *url){
    struct sockaddr_in server_addr;
    struct hostent *server;

    server = gethostbyname(url);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        exit(EXIT_FAILURE);
    }
     memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    

    int socketConnect = connect(socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(socketConnect < 0){
        perror("Error while connecting");
        exit(EXIT_FAILURE);
    }
}

void socketSend(int socketfd, const char *url, const char *method){
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    if((strncmp(method, "GET", 3)) == 0){   
         snprintf(message, sizeof(message),
             "GET / HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
              url);
    }else if(strncmp(method, "POST", 4) == 0){
        printf("Unsupported Method!\n");
        exit(EXIT_FAILURE);
    }

    int sendMessage = send(socketfd, message, strlen(message), 0);
    if(sendMessage < 0){
        perror("Error while sending message");
        exit(EXIT_FAILURE);
    }
}

void socketRecv(int socketfd){
    char buffer[2056];
    int socket_recv = recv(socketfd, buffer, sizeof(buffer) -1, 0);
    if(socket_recv < 0){
        perror("Error for receiving data");
        exit(EXIT_FAILURE);
    }
    buffer[socket_recv] = '\0';
    printf("%s\n",buffer); 
}
