/**
 * image test
 * result:
 * ```
 * 123
 * 123
 * fileB
 * ```
 */
const image = bakery.createImage();

image.onload = function() {
  log(123);
}
image.src = "fileA";
image.src = "fileB";

log(image.src)
