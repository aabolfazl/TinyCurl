//
// Created by Abolfazl Abbasi on 1.08.2024.
//
#ifndef TCURL_SOCKET_H
#define TCURL_SOCKET_H
int socketCreateConnect(const char *hostname,const char *port);
void httpRequest(int socketfd, const char *method, const char *hostname,const char *path, const char *data);
void recvRequest(int socketfd);
#endif //TCURL_SOCKET_H
