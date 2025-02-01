#ifndef BAKERYCANVAS_MODULE_H
#define BAKERYCANVAS_MODULE_H

#include "utils/v8pp_utils.hpp"
#include <unordered_map>

using v8::Context;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::Module;
using v8::Object;
using v8::Persistent;
using v8::Promise;
using v8::ScriptCompiler;
using v8::ScriptOrigin;
using v8::ScriptOrModule;
using v8::String;
using v8::TryCatch;
using v8::Value;

class BKModule {
    public:
    explicit BKModule();
    ~BKModule();
    Local<Module> load(Local<Context> context, const std::string& entry, std::string& exceptionStr);
    static std::string resolve(Isolate* isolate, const std::string& entryName, const std::string& currentPath, std::string& exceptionStr);

    static BKModule* instance() {
        static auto instance = new BKModule();
		return instance;
    };

    std::unordered_map<std::string, Persistent<Module>*>* moduleMap;
    private:

};

#endif  // BAKERYCANVAS_MODULE_H
