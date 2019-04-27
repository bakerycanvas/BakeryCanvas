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

#include "jsinternals/log.h"
#include "jsinternals/settimeout.h"
#include "jsinternals/requestanimationframe.h"
#include "jsinternals/bakery.h"

void Bind_Internals(v8::Isolate* iso);