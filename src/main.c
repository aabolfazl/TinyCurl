#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include "../libs/arg_parse/arg_parse.h"
#include "socket.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

static const char *const usages[] = {
    "tcurl [options] <url>",
    NULL,
};

int main(int argc, const char **argv) {
    const char *url = NULL;
    const char *method = NULL;
    const char *port = "80";
    const char *data = argc == 4 ? argv[3] : ""; 
    char hostname[256];
    char path[256];
    const char *user_agent = NULL;
    int timeout = 30;
    const char *request_method = "GET";
    const char *prefix = "http://";
    // https://3426f4e2-5ba9-473a-bb4d-e221023be581.mock.pstmn.io


    // GET /sayhello HTTP/1.1
    // Host: 3426f4e2-5ba9-473a-bb4d-e221023be581.mock.pstmn.io
    // User-Agent: curl/8.6.0
    // Accept: */*

    //    tcurl http://google.com/

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('A', "user-agent", &user_agent, "Specify the User-Agent string", NULL, 0, 0),
        OPT_INTEGER('m', "max-time", &timeout, "Maximum time allowed for the transfer", NULL, 0, 0),
        OPT_STRING('X', "request", &request_method, "Specify request method to use", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse,
                      "\nA simple curl-like HTTP client.",
                      "\nThis program sends HTTP requests and displays the responses.");

    argc = argparse_parse(&argparse, argc, argv);

    // The remaining argument should be the URL
    if(argv[0] == NULL){
        printf("NULL\n");
        return -1;
    }
    url = argv[0];
    if(strncmp(url,prefix,7)==0){
            url += 7;
        }
    method = argv[1];
    if(method == NULL){
        method = "GET";
    }
    parseUrl(url,hostname,path);
    
    int sockfd = createSocket();


    struct sockaddr_in server_addr;

    if (isIpAddress(hostname)) {
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(port));
        if (inet_pton(AF_INET, hostname, &server_addr.sin_addr) <= 0) {
            errorExit("Invalid IP address");
        }
        connectSocket(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    } else {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(hostname, port, &hints, &res) != 0) {
            errorExit("getaddrinfo failed");
        }
        connectSocket(sockfd, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
    }


    char request[BUFFER_SIZE];
    receiveHttpRequest(method, hostname, path, data, request);
    sendRequest(sockfd, request);
    receiveResponse(sockfd);
    close(sockfd);
    

    return 0;
}

/*
 * implement GET (https://www.w3schools.com/nodejs/nodejs_http.asp?pss=2e3)
 *
 * steps:
    1- get parameter from user
    2- basic url handling
    3- create http request
    4- send request
    5- start timeout timer
    6- print response/error
 * */
