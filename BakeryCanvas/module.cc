#include "module.h"
#include "logger.h"
#include "system.h"
#include <fstream>
#include <sstream>

Local<Module> BKModule::load(Local<Context> context, const std::string& entry) {
    auto isolate = context->GetIsolate();
    Context::Scope scope(context);
    ScriptOrigin origin(
        String::NewFromUtf8(isolate, entry.c_str()),  // name
        Integer::New(isolate, 0),                     // line offset
        Integer::New(isolate, 0),                     // column offset
        False(isolate),                               // is cross origin
        Local<Integer>(),                             // script id
        Local<Value>(),                               // Sourcemap URL
        False(isolate),                               // is opaque
        False(isolate),                               // is WASM
        True(isolate)                                 // is ES6 module
    );

    std::string scriptText;
    std::ifstream file;
    file.open(entry);

    if (!file) {
        std::string head = "Failed to read file ";
        auto message = head + entry;
        Logger::fatal(message);
        BKSystem::showMessage("Bakery Canvas Exception", message.c_str(), BKSystem::MessageLevel::ERROR);
        v8pp::throw_ex(isolate, "Bakery Canvas Exception" + message);
    }

    std::ostringstream tmp;
    tmp << file.rdbuf();
    file.close();
    scriptText = tmp.str();

    ScriptCompiler::Source source(String::NewFromUtf8(isolate, scriptText.c_str()), origin);
    Local<Module> module;
    if (!ScriptCompiler::CompileModule(isolate, &source).ToLocal(&module)) {
        // if you have a TryCatch, you should check it here.
        v8pp::throw_ex(isolate, "Bakery Canvas Exception");
    }

    // You can resolve import requests ahead of time (useful for async)
    //    for (int i = 0; i < module->GetModuleRequestsLength(); i++) {
    //        Local<String> specifier = module->GetModuleRequest(i);  // "some thing"
    //    }

    // or you can resolve them sync in the InstantiateModule callback
    module->InstantiateModule(
        context, [](Local<Context> context,   // "main.mjs"
                    Local<String> specifier,  // "some thing"
                    Local<Module> referrer) {
            auto entry = v8pp::from_v8<std::string>(context->GetIsolate(), specifier);
            Logger::debug(entry);
            auto module = BKModule::load(context, entry);
            return MaybeLocal<Module>(module);
        });

    // setting this callback enables dynamic import
    isolate->SetHostImportModuleDynamicallyCallback([](Local<Context> context, Local<ScriptOrModule> referrer, Local<String> specifier) { return MaybeLocal<Promise>(); });

    // setting this callback enables import.meta
    isolate->SetHostInitializeImportMetaObjectCallback([](Local<Context> context, Local<Module> module, Local<Object> meta) {
        // meta->Set(key, value); you could set import.meta.url here
    });

    Local<Value> result;
    if (module->Evaluate(context).ToLocal(&result)) {
        String::Utf8Value utf8(isolate, result);
        Logger::info("Script output: {:}", *utf8);
    } else {
        // once again, if you have a TryCatch, use it here.
    }

    return module;
}

void BKModule::run(Local<Module> module) {
}
