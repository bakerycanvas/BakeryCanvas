#include "settimeout.h"

namespace bkinternals {
  int setTimeout(v8::FunctionCallbackInfo<v8::Value> const &args, int time) {
    int* self = v8pp::class_<int>::unwrap_object(args.GetIsolate(), args.This());
    if (self)
      args.GetReturnValue().Set(*self);
    else
      args.GetReturnValue().Set(args[0]);
    // printf("%d, %s", *self, args[0]);
  };
}