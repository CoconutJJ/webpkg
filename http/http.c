#include "http.h"
#include "datarwbuffer.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t read_callback (char *dest, size_t size, size_t nmemb, void *data)
{
        HTTPRequest *request = data;
        size_t total_size = size * nmemb;

        return read_databuffer (&request->request_data, dest, total_size);
}

static size_t write_callback (char *dest, size_t size, size_t nmemb, void *data)
{
        HTTPRequest *request = data;
        size_t total_size = size * nmemb;

        return write_databuffer (&request->response_data, dest, total_size);
}

void init_HTTPRequest (HTTPRequest *request, char *url)
{
        request->url = url;
        request->error = NULL;
        request->headers = NULL;
        request->headers_capacity = 0;
        request->headers_count = 0;

        init_databuffer (&request->request_data);
        init_databuffer (&request->response_data);
}

void *http_post (HTTPRequest *request)
{
        CURL *curl = curl_easy_init ();

        if (!curl)
                return NULL;

        curl_easy_setopt (curl, CURLOPT_URL, request->url);
        curl_easy_setopt (curl, CURLOPT_POST, 1L);
        curl_easy_setopt (curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt (curl, CURLOPT_READDATA, request);
        curl_easy_setopt (curl, CURLOPT_POSTFIELDSIZE, request->request_data.count);

        CURLcode result = curl_easy_perform (curl);


        if (result != CURLE_OK) {
                request->error = curl_easy_strerror (result);
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &request->http_response_code);

        curl_easy_cleanup (curl);
}

void *http_get (HTTPRequest *request)
{
        CURL *curl = curl_easy_init ();

        if (!curl)
                return NULL;

        curl_easy_setopt (curl, CURLOPT_URL, request->url);
        curl_easy_setopt (curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, request);
        curl_easy_setopt (curl, CURLOPT_USERAGENT, "webpkg");
        CURLcode result = curl_easy_perform (curl);

        if (result != CURLE_OK) {
                request->error = curl_easy_strerror (result);
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &request->http_response_code);

        curl_easy_cleanup (curl);
}