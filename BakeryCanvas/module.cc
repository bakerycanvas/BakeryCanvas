#include "module.h"
#include "logger.h"
#include "system.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>

BKModule::BKModule() {
    this->moduleMap = new std::unordered_map<std::string, Persistent<Module>*>();
}

Local<Module> BKModule::load(Local<Context> context, const std::string& entry, std::string& exceptionStr) {
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
        exceptionStr = message;
        return Local<Module>();
    }

    std::ostringstream tmp;
    tmp << file.rdbuf();
    file.close();
    scriptText = tmp.str();

    TryCatch tryHandler(isolate);
    ScriptCompiler::Source source(String::NewFromUtf8(isolate, scriptText.c_str()), origin);
    Local<Module> module;
    if (!ScriptCompiler::CompileModule(isolate, &source).ToLocal(&module)) {
        v8::Local<v8::Value> exception = tryHandler.StackTrace(context).ToLocalChecked();
        v8::String::Utf8Value exception_utf8(isolate, exception);
        exceptionStr = *exception_utf8;
        return module;
    }

    // You can resolve import requests ahead of time (useful for async)
    for (int i = 0; i < module->GetModuleRequestsLength(); i++) {
        Local<String> specifier = module->GetModuleRequest(i);
        auto entry = v8pp::from_v8<std::string>(isolate, specifier);
        auto resolvedEntry = this->resolve(isolate, entry, "", exceptionStr);
        if (!exceptionStr.empty()) {
            return module;
        }
        Logger::debug("[Module] Pre-load module {:} from {:}", entry, resolvedEntry);
        auto module = this->load(context, resolvedEntry, exceptionStr);
        if (!exceptionStr.empty()) {
            return module;
        }
        this->moduleMap->insert(std::make_pair(resolvedEntry, new Persistent<Module>(isolate, module)));
    }

    // or you can resolve them sync in the InstantiateModule callback
    module->InstantiateModule(
        context,                     // context
        [](Local<Context> context,   // "main.mjs"
           Local<String> specifier,  // "some thing"
           Local<Module> referrer) {
            auto isolate = context->GetIsolate();
            auto entry = v8pp::from_v8<std::string>(isolate, specifier);
            std::string exceptionStr;
            auto resolvedEntry = BKModule::resolve(isolate, entry, "", exceptionStr);
            if (!exceptionStr.empty()) {
                Logger::warn("[Module] Cannot find pre-loaded module of {:}", resolvedEntry);
                return MaybeLocal<Module>();
            }
            Logger::debug("[Module] Load module {:} from {:}", entry, resolvedEntry);
            auto module = BKModule::instance()->moduleMap->find(resolvedEntry);
            if (module == BKModule::instance()->moduleMap->end()) {
                v8pp::throw_ex(isolate, "[Module] Cannot find pre-loaded module of " + resolvedEntry);
            }
            auto _module = Local<Module>::New(isolate, *(module->second));
            return MaybeLocal<Module>(_module);
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
        v8::MaybeLocal<v8::Value> maybe_exception = tryHandler.StackTrace(context);
        v8::Local<v8::Value> exception;
        if (maybe_exception.IsEmpty()) {
            exception = tryHandler.Exception();
        } else {
            exception = maybe_exception.ToLocalChecked();
        }
        v8::String::Utf8Value exception_utf8(isolate, exception);
        exceptionStr = *exception_utf8;
    }

    return module;
}

/**
 * Approximately equals to require.resolve in Node.js.
 * Reference: https://nodejs.org/api/modules.html#modules_all_together
 */
std::string BKModule::resolve(Isolate* isolate, const std::string& entryName, const std::string& currentPath, std::string& exceptionStr) {
    namespace fs = boost::filesystem;
    fs::path entry(entryName);
    auto cwd = currentPath.empty() ? fs::current_path() : fs::path(currentPath);

    if (entryName.rfind("/", 0) == 0) {
        const char* errorMessage = "[Module] Cannot load a module from file system root for security issues.";
        Logger::fatal(errorMessage);
        v8pp::throw_ex(isolate, errorMessage);
        exceptionStr = errorMessage;
        return "";
    } else if (entryName.rfind(".", 0) == 0) {
        // resolve to absolute path
        entry = fs::absolute(entry, cwd);
        if (fs::is_directory(entry)) {
            if (fs)
            auto entry2 = entry.appe
        }
        if (fs::exists(entry)) {
            return
        }
    } else {
        // resolve to node_modules
    }

    return entry.string();
}
