#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include "../libs/arg_parse/arg_parse.h"
#include "socket.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096


void parse_url(const char *url, char *hostname, char *path) {
    const char *slash = strchr(url, '/');
    if (slash) {
        strncpy(hostname, url, slash - url);
        hostname[slash - url] = '\0';
        strcpy(path, slash);
    } else {
        strcpy(hostname, url);
        strcpy(path, "/");
    }
}


static const char *const usages[] = {
    "tcurl [options] <url>",
    NULL,
};

int main(int argc, const char **argv) {
    const char *user_agent = NULL;
    int timeout = 30;
    const char *request_method = "GET";

    const char *url = argv[1];
    const char *method = argv[2];
    const char *port = "80";
    const char *data = argv[3];
    char hostname[256];
    char path[256];
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

    parse_url(url,hostname,path);
    printf("Host: %s\n",hostname);
    printf("Method: %s\n",method);
    printf("data: %s\n",data);
    printf("path: %s\n",path);
 
    int Socket = socketCreateConnect(hostname, port);
    httpRequest(Socket, method, hostname, path, data);
    recvRequest(Socket);

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
