#ifndef BAKERYCANVAS_MODULE_H
#define BAKERYCANVAS_MODULE_H

#include "utils/v8pp_utils.hpp"

using v8::Context;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::Module;
using v8::Object;
using v8::Promise;
using v8::ScriptCompiler;
using v8::ScriptOrigin;
using v8::ScriptOrModule;
using v8::String;
using v8::Value;

class BKModule {
    public:
    static Local<Module> load(Local<Context> context, const std::string& entry);
    static void run(Local<Module> module);
};

#endif  // BAKERYCANVAS_MODULE_H
