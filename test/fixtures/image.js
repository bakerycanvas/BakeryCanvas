/**
 * image test
 * result:
 * ```
 * 123
 * 123
 * fileB
 * ```
 */

function main() {
  let image = bakery.createImage();
  
  image.onload = function() {
    console.log(123);
  }
  image.src = "fileA";
  image.src = "fileB";
  
  console.log(image.src)
  
  image = null;
}

for (let i = 0; i < 100000; i++) {
  main();
}

const image2 = bakery.createImage();
