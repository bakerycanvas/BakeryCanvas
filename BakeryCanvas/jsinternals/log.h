#pragma once

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
#include "logger.h"

namespace BKJSInternals {
    namespace Console {
        void init(v8pp::module target);
        void log(const v8::FunctionCallbackInfo<v8::Value>& args);
    }
}