#ifndef BAKERYCANVAS_REQUIRE_H
#define BAKERYCANVAS_REQUIRE_H

#include "utils/v8pp_utils.hpp"

namespace BKJSInternals {
    void require(const v8::FunctionCallbackInfo<v8::Value>& args);
}

#endif  // BAKERYCANVAS_REQUIRE_H
