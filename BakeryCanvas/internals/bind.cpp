#include "bind.h"

void Bind_Internals(v8::Isolate* isolate) {
  v8::Local<v8::Function> v8_log = v8pp::wrap_function(isolate, "log", &bkinternals::log);
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "log"), v8_log);
  v8::Local<v8::Function> v8_setTimeout = v8pp::wrap_function(isolate, "setTimeout", &bkinternals::setTimeout);
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "setTimeout"), v8_setTimeout);
  v8::Local<v8::Function> v8_setInterval = v8pp::wrap_function(isolate, "setInterval", &bkinternals::setInterval);
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "setInterval"), v8_setInterval);
  v8::Local<v8::Function> v8_clearTimeout = v8pp::wrap_function(isolate, "clearTimeout", &bkinternals::clearTimeout);
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "clearTimeout"), v8_clearTimeout);
  v8::Local<v8::Function> v8_clearInterval = v8pp::wrap_function(isolate, "clearInterval", &bkinternals::clearInterval);
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "clearInterval"), v8_clearInterval);
}
