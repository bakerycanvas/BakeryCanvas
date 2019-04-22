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

#include "queue/queue.h"

namespace BKJSInternals {
  int setTimeout(v8::Local<v8::Function> callback, int time);
  int setInterval(v8::Local<v8::Function> callback, int time);
  void clearTimeout(int timerId);
  void clearInterval(int timerId);
  int _createTimer(bool repeat, v8::Local<v8::Function> callback, int time);
  void _timer_callback(uv_timer_t* timer);
}  // namespace BKJSInternals