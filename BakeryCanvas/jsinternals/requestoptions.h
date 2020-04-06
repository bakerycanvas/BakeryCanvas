#ifndef BAKERYCANVAS_REQUESTOPTIONS_H
#define BAKERYCANVAS_REQUESTOPTIONS_H

#include "utils/v8pp_utils.hpp"

namespace BKJSInternals {

    class RequestOptions {
        public:
        explicit RequestOptions(
            v8::Isolate* isolate, std::string url, std::string method, std::string dataType, std::string responseType, v8::Local<v8::Object> data, v8::Local<v8::Object> headers,
            v8::Local<v8::Function> onSuccess, v8::Local<v8::Function> onError);
        ~RequestOptions();

        std::string url;
        std::string method;
        std::string dataType;
        std::string responseType;
        v8::Persistent<v8::Object>* data;
        v8::Persistent<v8::Object>* headers;
        v8::Persistent<v8::Function>* onSuccess;
        v8::Persistent<v8::Function>* onError;
    };
}  // namespace BKJSInternals

#endif  // BAKERYCANVAS_REQUESTOPTIONS_H
