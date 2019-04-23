#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <uv.h>

namespace BKQueue {
  int start();
  int tick();
  int close();
  uv_timer_t* createTimer(uv_timer_cb callback, int time);
  void destoryTimer(uv_timer_t* timer);
  void _afterHandleClose(uv_handle_t* timer);
}