/**
 * 最基本测试，直接暴露 gl 对象到 v8 执行上下文
 * 尝试用 js 直接调用
 * 结果：显示胭脂红色画面
 */

setInterval(() => {
  gl.clearColor(Math.random(), Math.random(), Math.random(), 1.0);
  gl.clear(gl.GL_COLOR_BUFFER_BIT);
}, 1000);
