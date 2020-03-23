// const canvas = document.getElementById('canvas');
// const canvas = bakery.createCanvas();
// const ctx = canvas.getContext('2d');

console.log(CanvasRenderingContext2d, '\n')
// console.log(Canvas, '\n')

const canvas = bakery.createCanvas();
console.log(canvas.width)
const ctx = new CanvasRenderingContext2d(canvas);

const { Image } = bakery;

console.log(Image)


const image = new Image();
// console.log(image.onload)
image.src = '';

CanvasRenderingContext2dInit(() => {}, () => {
  return {
    weight: 'normal',
    style: 'medium',
    stretch: 'normal',
    variant: 'normal',
    size: 40,
    unit: 600,
    family: '冬青黑简体'
  }
});


console.log(canvas, image, ctx)
ctx.textDrawingMode = 'glyph'
ctx.font = '10px Impact'
var text = ctx.measureText('Awesome!')

console.log(text.width, '\n')

// ctx.rotate(0.1)
ctx.fillText('Awesome!', 10, 100)


ctx.lineWidth = 10;

// Wall
ctx.strokeRect(75, 140, 150, 110);

// Door
ctx.fillRect(130, 190, 40, 60);

console.log(ctx.font)
ctx.fillText('测试', 0, 200);

ctx.drawImage(image, 10, 10)

// Roof
ctx.moveTo(50, 140);
ctx.lineTo(150, 60);
ctx.lineTo(250, 140);
ctx.closePath();
ctx.stroke();

canvas.saveToPNG("test.png")
