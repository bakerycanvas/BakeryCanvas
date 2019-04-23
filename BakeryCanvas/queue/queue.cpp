#include "queue.h"

namespace BKQueue {
  int start() {
    return 0;
  }

  int tick() {
    return uv_run(uv_default_loop(), UV_RUN_NOWAIT);
  }

  int close() {
    return uv_loop_close(uv_default_loop());
  }

  uv_timer_t* createTimer(uv_timer_cb callback, int time) {
    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(uv_default_loop(), timer);
    uv_timer_start(timer, callback, time, time);
    return timer;
  }

  void destoryTimer(uv_timer_t* timer) {
    uv_timer_stop(timer);
    uv_close((uv_handle_t *)timer, _afterHandleClose);
  }

  void _afterHandleClose(uv_handle_t* handle) {
    free(handle);
  }

}  // namespace BKQueue
