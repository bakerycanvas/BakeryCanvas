#include "requestoptions.h"

namespace BKJSInternals {
    RequestOptions::RequestOptions(
        v8::Isolate* isolate, std::string url, std::string method, std::string dataType, std::string responseType, v8::Local<v8::Object> data, v8::Local<v8::Object> headers,
        v8::Local<v8::Function> onSuccess, v8::Local<v8::Function> onError) {
        this->url = url;
        this->method = method;
        this->dataType = dataType;
        this->responseType = responseType;

        std::transform(this->method.begin(), this->method.end(), this->method.begin(), ::toupper);

        this->data = new v8::Persistent<v8::Object>(isolate, data);
        this->headers = new v8::Persistent<v8::Object>(isolate, headers);
        this->onSuccess = new v8::Persistent<v8::Function>(isolate, onSuccess);
        this->onError = new v8::Persistent<v8::Function>(isolate, onError);
    }

    RequestOptions::~RequestOptions() {
    }

}  // namespace BKJSInternals