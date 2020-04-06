#ifndef BAKERYCANVAS_GOT_H
#define BAKERYCANVAS_GOT_H

#include "curl/curl.h"
#include <iostream>
#include <string>
#include "queue/queue.h"

namespace Got {

    extern char error_buffer[CURL_ERROR_SIZE];
    extern int writer(char*, size_t, size_t, std::string*);
    extern bool init(CURL*&, const char*, std::string*);

    extern bool init(CURL*& conn, const char* url, std::string* p_buffer);

    extern int writer(char* data, size_t size, size_t nmemb, std::string* writer_data);

    int get(const char* url, std::string& buffer, std::string& errinfo);

    int post(const char* url, const char* data, std::string& buffer, std::string& errinfo);

}  // namespace Got
#endif  // BAKERYCANVAS_GOT_H
