#include "log.h"

namespace BKJSInternals {
  const char* ToCString(const v8::String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
  }
  void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
    for (int i = 0; i < args.Length(); i++) {
      v8::HandleScope handle_scope(args.GetIsolate());
      v8::Local<v8::Value> value(args[ i ]);
      auto string = value->ToString(v8::Isolate::GetCurrent());
      auto x = v8pp::from_v8<const char*>(v8::Isolate::GetCurrent(), string);
      printf("%s ", x.c_str());
    }
    printf("\n");
  };
}  // namespace BKJSInternals