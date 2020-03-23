#include "log.h"
#include "printf.h"
#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/spdlog.h>
#include <string>

namespace BKJSInternals {
    namespace Console {
        //        std::vector<char> formatMark = { 's', 'f', 'd', 'i' };
        void init(v8pp::module target) {
            auto isolate = v8::Isolate::GetCurrent();
            v8pp::module module(isolate);
            module.set("log", log);
            module.set("info", log);
            module.set("debug", log);
            module.set("warn", log);
            module.set("error", log);

            target.set("console", module);
        }
        void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
            std::string rawFormat;
            std::vector<std::string> logArgs;
            for (int i = 0; i < args.Length(); i++) {
                v8::HandleScope handle_scope(args.GetIsolate());
                v8::Local<v8::Value> value(args[i]);
                auto string = value->ToString(v8::Isolate::GetCurrent());
                auto x = v8pp::from_v8<std::string>(v8::Isolate::GetCurrent(), string);
                if (i == 0) {
                    rawFormat = x;
                } else {
                    logArgs.push_back(x);
                }
            }

            std::string format = "";
            bool symbolScan = false;
            bool precisionScan = false;
            int argPosition = 0;
            std::string precision;
            for (int j = 0; j < rawFormat.length(); j++) {
                auto currentChar = rawFormat[j];
                if (symbolScan && !precisionScan && currentChar == '.') {
                    precisionScan = true;
                    precision.push_back('.');
                } else if (symbolScan && precisionScan && currentChar >= '0' && currentChar <= '9') {
                    precision.push_back(currentChar);
                } else if (symbolScan) {
                    symbolScan = false;
                    precisionScan = false;

                    if (argPosition < logArgs.size()) {
                        switch (currentChar) {
                            case 's':
                            case 'f':
                            case 'd':
                            case 'i':
                                format += logArgs[argPosition++];
                                break;
                            default:
                                format += "%";
                                format += precision;
                                format += currentChar;
                                break;
                        }
                    } else {
                        format += "%";
                        format += precision;
                        format += currentChar;
                    }

                    //                    format += "{:";
                    //                    format += precision;
                    //                    format += currentChar;
                    //                    format += "}";

                } else if (currentChar == '%') {
                    symbolScan = true;
                    precision.clear();
                } else {
                    format += currentChar;
                }
            }

            Logger::Console::info(format);
        };
    }  // namespace Console
}  // namespace BKJSInternals