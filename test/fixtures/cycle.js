/**
 * setInterval 测试：每秒切换画面颜色
 */

setInterval(() => {
  gl.clearColor(Math.random(), Math.random(), Math.random(), 1.0);
  gl.clear(gl.GL_COLOR_BUFFER_BIT);
}, 1000);
