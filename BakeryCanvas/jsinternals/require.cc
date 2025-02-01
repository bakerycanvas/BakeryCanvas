#include "require.h"
#include <filesystem>

namespace BKJSInternals {
    void require(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handle_scope(args.GetIsolate());
        if (args.Length() < 1) {
            v8pp::throw_ex(isolate, "must pass a argument");
            return;
        }
        v8::Local<v8::Value> value(args[0]);
        auto path = value->ToObject(isolate);
        if (!path->IsString()) {
            v8pp::throw_ex(isolate, "path must be a string.");
            return;
        }

//        v8::Module::Evaluate()

    }
}
