const vertices = [
    -0.5, -0.5, 0.0,
     0.5, -0.5, 0.0,
     0.0,  0.5, 0.0
];

log("define vertices");

// const canvas = document.querySelector('#glcanvas');
const canvas = bakery.createCanvas();
const gl = canvas.getContext('webgl');

const verticesBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, verticesBuffer)
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

log("create buffer");

// Vertex shader program
const vsSource = `
attribute vec3 aPos;
void main(void) {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
`;
// const vsSource = `
// #version 330

// in vec3 _uaPos;
// void main(){
// gl_Position = vec4(_uaPos.x, _uaPos.y, _uaPos.z, 1.0);
// }
// `;

// Fragment shader program
const fsSource = `
void main(void) {
    gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
`;
// const fsSource = `
// #version 330
// #extension GL_ARB_gpu_shader5 : enable
// out vec4 webgl_FragColor;
// void main(){
// (webgl_FragColor = vec4(1.0, 0.5, 0.2, 1.0));
// }
// `;

const shaderProgram = initShaderProgram(vsSource, fsSource);
const attrib_loc = 0;//gl.getAttribLocation(shaderProgram, 'aPos');

log("finish program");

var then = 0;

// Draw the scene repeatedly
function render(now) {
    now *= 0.001; // convert to seconds
    const deltaTime = now - then;
    then = now;

    drawScene(shaderProgram, verticesBuffer, attrib_loc);

    requestAnimationFrame(render);
}
requestAnimationFrame(render);

function drawScene(program, buffers, attrib_loc) {
    gl.clearColor(0.0, 0.0, 0.0, 1.0); // Clear to black, fully opaque
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers);
    gl.vertexAttribPointer(attrib_loc, 3, gl.FLOAT, false, 12, 0);
    gl.enableVertexAttribArray(attrib_loc);
    gl.useProgram(program);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
}

function initShaderProgram(vsSource, fsSource) {
  const vertexShader = loadShader(gl.VERTEX_SHADER, vsSource);
  const fragmentShader = loadShader(gl.FRAGMENT_SHADER, fsSource);

  // Create the shader program

  const shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  // If creating the shader program failed, alert

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    log('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
    return null;
  }

  return shaderProgram;
}

//
// creates a shader of the given type, uploads the source and
// compiles it.
//
function loadShader(type, source) {
  const shader = gl.createShader(type);

  // Send the source to the shader object

  gl.shaderSource(shader, source);

  // Compile the shader program

  gl.compileShader(shader);

  // See if it compiled successfully

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    log('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}
