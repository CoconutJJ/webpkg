#ifndef http_h
#define http_h
#include <stdlib.h>
#include "datarwbuffer.h"

typedef struct {
        char *key;
        char *value;

} HTTPHeader;


typedef struct {

        char *url;

        const char *error;

        HTTPHeader *headers;
        size_t headers_count;
        size_t headers_capacity;

        DataRWBuffer request_data;

        long http_response_code;
        DataRWBuffer response_data;
        


} HTTPRequest;

void init_HTTPRequest (HTTPRequest *request, char *url);
void destroy_HTTPRequest(HTTPRequest *request);
void *http_post (HTTPRequest *request);
void *http_get (HTTPRequest *request);
#endif