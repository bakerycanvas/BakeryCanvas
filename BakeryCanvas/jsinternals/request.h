#ifndef BAKERYCANVAS_REQUEST_H
#define BAKERYCANVAS_REQUEST_H

#include "queue/queue.h"
#include "requestoptions.h"
#include "utils/got.h"
#include "utils/v8pp_utils.hpp"

namespace BKJSInternals {
    void request(const v8::FunctionCallbackInfo<v8::Value>& args);
    void _request_runner(
        v8::Isolate* isolate, std::string url, std::string method, std::string dataType, std::string responseType, v8::Local<v8::Object> data, v8::Local<v8::Object> headers,
        v8::Local<v8::Function> onSuccess, v8::Local<v8::Function> onError);
    void _request_callback(uv_async_t* handle);
    void _request_close(uv_handle_t* handle);
}  // namespace BKJSInternals

#endif  // BAKERYCANVAS_REQUEST_H
