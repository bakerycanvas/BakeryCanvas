#include "requestanimationframe.h"

namespace BKJSInternals {
  struct CallbackInfo {
    int64_t id;
    v8::Persistent<v8::Function>* callbackFunc;
  };

  bool init = false;
  bool excuting = false;
  uv_idle_t handle;

  int64_t id = 0;
  std::queue<CallbackInfo*>* queue;
  std::queue<CallbackInfo*>* nextQueue;
  std::set<int64_t>* cancelledId;

  int64_t requestAnimationFrame(v8::Local<v8::Function> callback) {
    if (!init) {
      init = true;
      queue = new std::queue<CallbackInfo*>();
      nextQueue = new std::queue<CallbackInfo*>();
      cancelledId = new std::set<int64_t>();

      uv_idle_init(uv_default_loop(), &handle);
      uv_idle_start(&handle, _animationCallback);
    }

    auto isolate = v8::Isolate::GetCurrent();
    v8::Persistent<v8::Function>* func = new v8::Persistent<v8::Function>(isolate, callback);

    CallbackInfo* callbackInfo = (CallbackInfo*)malloc(sizeof(CallbackInfo));
    callbackInfo->id = ++id;
    callbackInfo->callbackFunc = func;

    if (excuting) {
      nextQueue->push(callbackInfo);
    } else {
      queue->push(callbackInfo);
    }

    return id;
  };
  void cancelAnimationFrame(int64_t id) {
    cancelledId->insert(id);
  }

  void _animationCallback(uv_idle_t* handle) {
    auto isolate = v8::Isolate::GetCurrent();
    excuting = true;
    int64_t lastId = 0;
    while (queue->size() > 0) {
      auto item = queue->front();
      queue->pop();
      lastId = item->id;
      if (cancelledId->find(item->id) != cancelledId->end()) {
        continue;
      }
      v8::HandleScope scope(isolate);
      v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(item->callbackFunc));
      v8pp::call_v8(isolate, callback, isolate->GetCurrentContext()->Global(), (double)clock() / CLOCKS_PER_SEC * 1000);
      item->callbackFunc->SetWeak();
      free(item);
    }
    if (lastId > 0) {
      auto r = cancelledId->lower_bound(lastId);
      cancelledId->erase(r, cancelledId->end());
    }
    excuting = false;
    auto swapV = queue;
    queue = nextQueue;
    nextQueue = swapV;
  }
}  // namespace BKJSInternals