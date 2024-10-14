#ifndef SEND_HTTP_REQUEST_H
#define SEND_HTTP_REQUEST_H
void send_http_request(int sockfd, const char *ip,const char *host, const char *method, const char *path);
#endif //SEND_HTTP_REQUEST_H
