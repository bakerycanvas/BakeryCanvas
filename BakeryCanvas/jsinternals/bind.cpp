#include "bind.h"

#define BIND_INTERNAL_FUNC_DIFF(v8name, localname)                                                         \
  v8::Local<v8::Function> v8_##v8name = v8pp::wrap_function(isolate, #v8name, &BKJSInternals::localname); \
  isolate->GetCurrentContext()->Global()->Set(v8::String::NewFromUtf8(isolate, #v8name), v8_##v8name);

#define BIND_INTERNAL_FUNC(x) BIND_INTERNAL_FUNC_DIFF(x,x)

void Bind_Internals(v8::Isolate* isolate) {
  auto global = isolate->GetCurrentContext()->Global();
  global->Set(v8::String::NewFromUtf8(isolate, "window"), global);
  BIND_INTERNAL_FUNC(log);
  BIND_INTERNAL_FUNC(setTimeout);
  BIND_INTERNAL_FUNC(setInterval);
  BIND_INTERNAL_FUNC(clearTimeout);
  BIND_INTERNAL_FUNC(clearInterval);
  BIND_INTERNAL_FUNC(requestAnimationFrame);
  BIND_INTERNAL_FUNC(cancelAnimationFrame);

  BKJSInternals::initBakery();  
}
