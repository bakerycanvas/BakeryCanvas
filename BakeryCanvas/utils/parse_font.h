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
#include <unordered_map>
#include <regex>

struct fontinfo{
    const char* weight;
    const char* style;
    const char* stretch;
    const char* variant;
    float size;
    const char* unit;
    const char* family;
};

fontinfo* parseCSSFont(const char* str);
