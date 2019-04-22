#include "queue.h"

namespace BKQueue {
  // uv_loop_t* loop;

  int start() {
    // loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    // uv_loop_init(loop);

    // uv_idle_t idler;

    // uv_idle_init(uv_default_loop(), &idler);
    // uv_idle_start(&idler, wait_for_a_while);

    // free(loop);
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
    uv_close((uv_handle_t *)timer, on_timer_close_complete);
  }

  void on_timer_close_complete(uv_handle_t* timer) {
    free(timer);
  }

}  // namespace BKQueue
