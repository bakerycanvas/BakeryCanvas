#include "settimeout.h"

namespace bkinternals {
  int setTimeout(v8::Local<v8::Function> cb, int time) {
    auto* isolate = v8::Isolate::GetCurrent();
    v8pp::call_v8(isolate, cb, isolate->GetCurrentContext()->Global());
  };
}  // namespace bkinternals
