/*
 * Copyright © 2024 Tiny Curl (tcurl) project
 *
 * This is the source code of the Tiny Curl (tcurl) project.
 * It is licensed under the MIT License; you should have received a copy
 * of the license in this archive (see LICENSE).
 *
 * Authors: Abolfazl Abbasi
 *
 */

#include <stdint.h>
#include <sys/socket.h>

typedef enum {
    HTTP_METH_GET,
} http_meth_t;

typedef enum {
    HTTP_STATUS_OK = 200,
} http_status_t;

typedef struct {
    size_t len;
    char *data;
} str_t;

typedef struct {
    size_t size;
    char *area;
    size_t data;
    size_t head;
} buffer_t;

struct http_req {
    http_meth_t meth;
    str_t path;
    str_t host;
    buffer_t body;
};

struct http_resp {
    http_status_t status;
    buffer_t body;
};