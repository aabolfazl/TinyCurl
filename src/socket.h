//
// Created by Abolfazl Abbasi on 1.08.2024.
//
#ifndef TCURL_SOCKET_H
#define TCURL_SOCKET_H

int createSocket();
void socketConnect(int socketfd);
void socketSend(int socketfd);
void socketRecv(int socketfd);
#endif //TCURL_SOCKET_H
