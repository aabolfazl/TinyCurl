//
// Created by Abolfazl Abbasi on 1.08.2024.
//

#ifndef TCURL_SOCKET_H
#define TCURL_SOCKET_H
#include <netinet/in.h>

void error_exit(const char *message);
void parse_url(const char *url, char *hostname, char *path);
int is_ip_address(const char *hostname);
int create_socket();
void connect_socket(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void send_request(int sockfd, const char *request);
void receive_response(int sockfd);
void build_http_request(const char *method, const char *hostname, const char *path, const char *data, char *request);

#endif //TCURL_SOCKET_H
