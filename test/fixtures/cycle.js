/**
 * setInterval 测试：每秒切换画面颜色
 */

let i = 0;
let id = setInterval(() => {
  const r = Math.random();
  const g = Math.random();
  const b = Math.random();
  log(`颜色变为 r=${r}   g=${g}    b=${b}`);
  gl.clearColor(r, g, b, 1.0);
  gl.clear(gl.GL_COLOR_BUFFER_BIT);
  i++;
  if (i > 2) {
    clearInterval(i);
  }
}, 1000);
