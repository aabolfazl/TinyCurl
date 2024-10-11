#include <ctype.h>
#include <stdio.h>
#include "../libs/arg_parse/arg_parse.h"
#include "socket.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "send_http_request.h"
int err = 0;

static const char *const usages[] = {
    "tcurl [options] <url>",
    NULL,
};

int main(int argc, const char **argv) {
    const char *url = NULL;
    const char *method = NULL;
    const char *data = NULL;
    const char *user_agent = NULL;
    int timeout = 30;
    const char *request_method = "GET";
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
    if (argc > 0) {
        url = argv[0];
        method = argv[1];
        data = argv[2];
    }
    // //NULL check
    if (url == NULL) {
        printf("url is NULL\n");
        exit(EXIT_SUCCESS);
    }

    send_http_request1(url,method,data);

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
