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

console.log(1);

setTimeout(() => {
  console.log(2);
  Promise.resolve().then(() => {
    console.log(3)
  });
}, 0);

new Promise((resolve, reject) => {
  console.log(4)
  resolve(5)
}).then((data) => {
  console.log(data);
})

setTimeout(() => {
  console.log(6);
}, 0)

console.log(7);

requestAnimationFrame(() => {
  requestAnimationFrame(() => {
    console.log(9);
  });
  let id = requestAnimationFrame(() => {
    console.log(9.1);
  });
  console.log(8);
  setTimeout(() => {
    console.log(10);
    cancelAnimationFrame(id);
  }, 0);
});
