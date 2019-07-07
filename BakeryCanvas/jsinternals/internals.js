const list = [
    'log',
    'setTimeout', 'clearTimeout',
    'setInterval', 'clearInterval',
    'requestAnimationFrame', 'cancelAnimationFrame',
    'Image', 'Canvas',
    'createImage', 'createCanvas',
];

for (const name of list) {
    window[name] = bakery[name];
}
