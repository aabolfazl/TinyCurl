#ifndef TCURL_SOCKET_H
#define TCURL_SOCKET_H
int create_socket(const char *ip);
void socket_connect(int __socketFileDesciptor,const char *ip);
void socket_send_recv(int createdSocket,const char *host, const char *method, const char *path); 
#endif //TCURL_SOCKET_H
