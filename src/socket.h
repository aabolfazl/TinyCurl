//
// Created by Abolfazl Abbasi on 1.08.2024.
//

#ifndef TCURL_SOCKET_H
#define TCURL_SOCKET_H
#include <netinet/in.h>

void errorExit(const char *message);
void parseUrl(const char *url, char *hostname, char *path);
int isIpAddress(const char *hostname);
int createSocket();
void connectSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void sendRequest(int sockfd, const char *request);
void receiveResponse(int sockfd);
void receiveHttpRequest(const char *method, const char *hostname, const char *path, const char *data, char *request);

#endif //TCURL_SOCKET_H
