#include "sys/socket.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "errno.h"
#include <string.h>

int createSocket(){
    int socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd < 0){
        perror("Error for creating socket");
        EXIT_FAILURE;
    }
    return socketfd;
}

void socketConnect(int socket){
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int socketConnect = connect(socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(socketConnect < 0){
        perror("Error while connecting");
        exit(EXIT_FAILURE);
    }
}

void socketSend(int socketfd){
    const char *message = "Message from Client!";
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
