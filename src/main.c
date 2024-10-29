//C library
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//system library
#include <sys/socket.h>


//project library
#include "../libs/arg_parse/arg_parse.h"
#include "socket.h"


//Definitions
#define BUFFER_SIZE 4096


int resolve_domain(const char *host, char *ip) {
    struct hostent *he;
    struct in_addr **addr_list;

    if ((he = gethostbyname(host)) == NULL) {
        perror("DNS resolution failed");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;
    strcpy(ip, inet_ntoa(*addr_list[0]));
    return 0;
}

static const char *const usages[] = {
    "tcurl [options] <url>",
    NULL,
};

int main(int argc, const char **argv) {
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char post_data[] = "key1=value1&key2=value2";
    char ip[100];
    const char *path = NULL;
    const char *method = NULL;
    const char *url = NULL;
    const char *output_file = NULL;
    int verbose = 0;
    int follow_redirects = 0;
    const char *user_agent = NULL;
    int timeout = 30; // Default timeout in seconds
    const char *request_method = "GET";

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
    
    if (argc == 0) {
        printf("Type -h for showing help menu\nUsage: <URL> <GET/POST> <path(optional)>\n");
        return EXIT_SUCCESS;
    }

    if (argc > 0) {
        url = argv[0];
        if(strncmp(url,"http://",7)==0){
            url += 7;
        }
        method = argv[1];
        if(method == NULL){
            method = request_method;
        }
        path = argv[2];
        if(path == NULL){
            path = "/";
        }
    }
    
    struct in_addr addr;
    if (inet_pton(AF_INET, url, &addr) == 1) {
            strcpy(ip, url);
    } else {
    if (resolve_domain(url, ip) != 0) {
            fprintf(stderr, "Failed to resolve domain: %s\n", url);
            return EXIT_FAILURE;
        }
    }

    //Code :
    int socket = create_socket(ip);     
    socket_connect(socket,ip); 
    socket_send_recv(socket,url,method,path);
    close(socket);  
    
    return 0;
}
