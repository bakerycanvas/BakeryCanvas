#include "request.h"
#include "logger.h"
#include "utils/got.h"

namespace BKJSInternals {

    void request(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handle_scope(args.GetIsolate());
        if (args.Length() < 1) {
            v8pp::throw_ex(isolate, "must pass a argument");
            return;
        }
        v8::Local<v8::Value> value(args[0]);
        auto options = value->ToObject(isolate);
        if (!options->IsObject()) {
            v8pp::throw_ex(isolate, "Options must be an object.");
            return;
        }
        v8::Local<v8::Object> data;
        std::string url;
        std::string method;
        v8::Local<v8::Object> headers;
        std::string dataType;
        std::string responseType;
        v8::Local<v8::Function> onSuccess;
        v8::Local<v8::Function> onError;
        v8pp::get_option(isolate, options, "data", data);
        v8pp::get_option(isolate, options, "url", url);
        v8pp::get_option_with_default(isolate, options, "method", method, std::string("GET"));
        v8pp::get_option(isolate, options, "header", headers);
        v8pp::get_option(isolate, options, "dataType", dataType);
        v8pp::get_option(isolate, options, "responseType", responseType);
        v8pp::get_option(isolate, options, "onSuccess", onSuccess);
        v8pp::get_option(isolate, options, "onError", onError);

        // TODO: validate params

        _request_runner(isolate, url, method, dataType, responseType, data, headers, onSuccess, onError);
    }

    void _request_runner(
        v8::Isolate* isolate, std::string url, std::string method, std::string dataType, std::string responseType, v8::Local<v8::Object> data, v8::Local<v8::Object> headers,
        v8::Local<v8::Function> onSuccess, v8::Local<v8::Function> onError) {
        auto* requestOptions = new RequestOptions(isolate, url, method, dataType, responseType, data, headers, onSuccess, onError);

        auto* handle = new uv_async_t;
        uv_async_init(uv_default_loop(), handle, _request_callback);
        handle->data = requestOptions;
        uv_async_send(handle);
    }

    void _request_callback(uv_async_t* handle) {
        auto* options = (RequestOptions*)handle->data;
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handle_scope(isolate);

        v8::Local<v8::Object> data = v8::Local<v8::Object>::New(isolate, *(options->data));

        auto url = options->url;
        auto method = options->method;
        v8::Local<v8::Object> headers = v8::Local<v8::Object>::New(isolate, *(options->headers));
        auto dataType = options->dataType;
        auto responseType = options->responseType;
        v8::Local<v8::Function> onSuccess = v8::Local<v8::Function>::New(isolate, *(options->onSuccess));
        v8::Local<v8::Function> onError = v8::Local<v8::Function>::New(isolate, *(options->onError));

        std::transform(method.begin(), method.end(), method.begin(), ::toupper);

        Logger::debug("[GOT] Requesting to {:}.", url);

        if (method == "GET") {
            std::string buffer;
            std::string err;

            Got::get(url.c_str(), buffer, err);

            if (err.empty() && !onSuccess.IsEmpty() && onSuccess->IsFunction()) {
                Logger::debug("[GOT] Success.");
                v8pp::call_v8(isolate, onSuccess, isolate->GetCurrentContext()->Global(), buffer);
            } else if (!err.empty() && !onError.IsEmpty() && onError->IsFunction()) {
                Logger::debug("[GOT] Error.");
                v8pp::call_v8(isolate, onError, isolate->GetCurrentContext()->Global(), v8::Exception::Error(v8pp::to_v8(isolate, err)));
            }
        } else {
            v8pp::throw_ex(isolate, "Unrecognized method of " + method);
        }

        delete options;
        uv_close((uv_handle_t*)handle, _request_close);
    }

    void _request_close(uv_handle_t* handle) {
        Logger::debug("[GOT] Complete.");
        delete (uv_async_t*)handle;
    }
}  // namespace BKJSInternals
