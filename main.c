#include <stdio.h>
#include "libs/arg_parse/arg_parse.h"
#include <string.h>
#include <sys/socket.h>

static const char *const usages[] = {
    "tcurl [options] <url>",
    NULL,
};

int main(int argc, const char **argv) {
    const char *url = NULL;
    const char *output_file = NULL;
    int verbose = 0;
    int follow_redirects = 0;
    const char *user_agent = NULL;
    int timeout = 30;  // Default timeout in seconds
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

    // The remaining argument should be the URL
    if (argc > 0) {
        url = argv[0];
    }

    if (url) {
        printf("URL: %s\n", url);
    }
    if (output_file) {
        printf("Output file: %s\n", output_file);
    }
    if (verbose) {
        printf("Verbose mode: on\n");
    }
    if (follow_redirects) {
        printf("Follow redirects: yes\n");
    }
    if (user_agent) {
        printf("User-Agent: %s\n", user_agent);
    }
    return 0;
}
