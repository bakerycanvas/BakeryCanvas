#include "settimeout.h"

namespace bkinternals {

  // Data structure to store info of a timer
  struct TimerInfo {
    int timerId;
    bool repeat;
    v8::Persistent<v8::Function>* callbackFunc;
  };

  std::unordered_map<int, uv_timer_t*> timerIdMap;
  std::unordered_map<uv_timer_t*, TimerInfo*> callbackMap;

  // always incresase
  int timerId = 0;

  int setTimeout(v8::Local<v8::Function> callback, int time = 4) {
    return _createTimer(false, callback, time);
  }

  int setInterval(v8::Local<v8::Function> callback, int time = 4) {
    return _createTimer(true, callback, time);
  }

  void clearTimeout(int timerId) {
    auto isolate = v8::Isolate::GetCurrent();
    auto timer = timerIdMap[timerId];
    auto timerInfo = callbackMap[timer];

    // ignore invalid timerId
    if (reinterpret_cast<size_t>(timer) <= 0) {
      return;
    }

    // remove timer from uv loop
    BKQueue::destoryTimer(timer);
    // clear store
    timerIdMap.erase(timerInfo->timerId);
    callbackMap.erase(timer);
    // release memory
    timerInfo->callbackFunc->SetWeak();
    free(timerInfo);
  }

  void clearInterval(int timerId) {
    clearTimeout(timerId);
  }

  int _createTimer(bool repeat, v8::Local<v8::Function> callback, int time) {
    // https://developer.mozilla.org/en-US/docs/Web/API/WindowOrWorkerGlobalScope/setTimeout#Minimum_delay_and_timeout_nesting
    if (time < 4) {
      time = 4;
    }

    uv_timer_t* timer = BKQueue::createTimer(_timer_callback, time);

    auto* isolate = v8::Isolate::GetCurrent();
    v8::Persistent<v8::Function>* func = new v8::Persistent<v8::Function>(isolate, callback);

    TimerInfo* timerInfo = (TimerInfo*)malloc(sizeof(TimerInfo));
    timerInfo->timerId = ++timerId;
    timerInfo->callbackFunc = func;
    timerInfo->repeat = repeat;

    timerIdMap.insert(std::make_pair(timerId, timer));
    callbackMap.insert(std::make_pair(timer, timerInfo));
    printf("%d", timerId);
    return timerId;
  }

  void _timer_callback(uv_timer_t* timer) {
    auto isolate = v8::Isolate::GetCurrent();
    auto timerInfo = callbackMap[timer];

    v8::HandleScope scope(isolate);
    v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(timerInfo->callbackFunc));

    // run callback
    v8pp::call_v8(isolate, callback, isolate->GetCurrentContext()->Global());

    if (!timerInfo->repeat) {
      // remove timer from uv loop
      BKQueue::destoryTimer(timer);
      // clear store
      timerIdMap.erase(timerInfo->timerId);
      callbackMap.erase(timer);
      // release memory
      timerInfo->callbackFunc->SetWeak();
      free(timerInfo);
    }
  }
}  // namespace bkinternals
