/**
 * Result:
 * ```
 * 1 
 * 4 
 * 7 
 * 5 
 * 2 
 * 3 
 * 6
 * 10
 * 9
 * ```
 */

log(1);

setTimeout(() => {
  log(2);
  Promise.resolve().then(() => {
    log(3)
  });
}, 0);

new Promise((resolve, reject) => {
  log(4)
  resolve(5)
}).then((data) => {
  log(data);
})

setTimeout(() => {
  log(6);
}, 0)

log(7);

requestAnimationFrame(() => {
  requestAnimationFrame(() => {
    log(9);
  });
  let id = requestAnimationFrame(() => {
    log(9.1);
  });
  log(8);
  setTimeout(() => {
    log(10);
    cancelAnimationFrame(id);
  }, 0);
});
