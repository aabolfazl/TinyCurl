//
// Created by Abolfazl Abbasi on 1.08.2024.
//

#ifndef TCURL_SOCKET_H
#define TCURL_SOCKET_H

void send_http_request(const char *host, const char *method, const char *data);

int socket_create(const char *host, int port);

int socket_connect();

int socket_getPost();

int socket_send();

int socket_recv();

int socket_close();


#endif //TCURL_SOCKET_H
