#include "bind.h"

void Bind_Internals(v8::Isolate * isolate) {
  v8::Local<v8::Function> v8_fun = v8pp::wrap_function(isolate, "log", &bkinternals::log);
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "log"), v8_fun);
  // v8::Local<v8::Function> v8_fun2 = v8pp::wrap_function(isolate, "setTimeout", &bkinternals::setTimeout);
  // isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, "setTimeout"), v8_fun2);
}
