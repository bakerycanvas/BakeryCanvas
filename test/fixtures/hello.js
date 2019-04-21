/**
 * 最基本测试，直接暴露 gl 对象到 v8 执行上下文
 * 尝试用 js 直接调用
 * 结果：显示胭脂红色画面
 */

gl.clearColor(0.8, 0.3, 0.3, 1.0);
gl.clear(gl.GL_COLOR_BUFFER_BIT);
