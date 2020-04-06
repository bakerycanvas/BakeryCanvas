#pragma once

#include "logger.h"
#include "v8pp/call_from_v8.hpp"
#include "v8pp/call_v8.hpp"
#include "v8pp/class.hpp"
#include "v8pp/config.hpp"
#include "v8pp/context.hpp"
#include "v8pp/json.hpp"
#include "v8pp/module.hpp"
#include "v8pp/object.hpp"
#include "v8pp/persistent.hpp"
#include "v8pp/throw_ex.hpp"
#include "v8pp/utility.hpp"

namespace BKJSInternals {
    namespace Console {
        void init(v8pp::module target);
        const std::string _print(const v8::FunctionCallbackInfo<v8::Value>& args);
        void log(const v8::FunctionCallbackInfo<v8::Value>& args);
        void info(const v8::FunctionCallbackInfo<v8::Value>& args);
        void debug(const v8::FunctionCallbackInfo<v8::Value>& args);
        void warn(const v8::FunctionCallbackInfo<v8::Value>& args);
        void error(const v8::FunctionCallbackInfo<v8::Value>& args);
    }  // namespace Console
}  // namespace BKJSInternals