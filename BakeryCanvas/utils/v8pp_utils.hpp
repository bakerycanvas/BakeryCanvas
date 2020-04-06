#ifndef BAKERYCANVAS_V8PP_UTILS_HPP
#define BAKERYCANVAS_V8PP_UTILS_HPP

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

namespace v8pp {
    template <typename T> bool get_option_with_default(v8::Isolate* isolate, v8::Local<v8::Object> options, char const* name, T& value, const T& defaultValue) {
        auto success = v8pp::get_option(isolate, options, name, value);
        if (!success) {
            value = defaultValue;
        }
        return true;
    }
}  // namespace v8pp

#endif  // BAKERYCANVAS_V8PP_UTILS_HPP
