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
#include <cstdint>
#include <queue>
#include <set>

#include "queue/queue.h"

namespace BKJSInternals {
  int64_t requestAnimationFrame(v8::Local<v8::Function> callback);
  void cancelAnimationFrame(int64_t timerId);
  void _animationCallback(uv_idle_t* handle);
}  // namespace BKJSInternals