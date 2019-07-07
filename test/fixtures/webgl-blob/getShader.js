const AllShaders = {};

function getShaderByName(name) {
  return AllShaders[name];
}

AllShaders.spinning_cube_fs = `
  #ifdef GL_ES
  precision mediump float;
  #endif
  varying vec4 v_color;
  void main(void) {
    gl_FragColor = v_color;
  }
`;

AllShaders.spinning_cube_vs = `
  attribute vec3 position;
  attribute vec3 normal;
  attribute vec2 texCoord;
  uniform mat4 u_worldviewproj;
  varying vec4 v_color;
  void main(void) {
    v_color = vec4(position.x + 0.5, position.y + 0.5, position.z + 0.5, 1.0);
    gl_Position = u_worldviewproj * vec4(position, 1.0);
  }
`;

AllShaders.marching_cube_fs = `
  #ifdef GL_ES
  precision mediump float;
  #endif
  varying vec4 v_color;
  varying vec3 v_normal;
  uniform vec4 u_ambientUp;
  uniform vec4 u_ambientDown;
  uniform vec3 u_lightDir;
  uniform vec4 u_lightColor;
  varying vec3 v_eyeDir;
  void main(void) {
    float light = max(0.0, dot(normalize(u_lightDir), v_normal));
    float glare = max(0.0, dot(v_normal, normalize(v_eyeDir + u_lightDir)));
    glare = glare*glare;
    glare = glare*glare;
    glare = glare*glare;
    glare = glare*glare;
    glare = glare*glare;
    glare = glare*glare;
    vec4 ambient = mix(u_ambientDown, u_ambientUp, (v_normal.y + 1.0)/2.0);
    // Gamma correction approximation (sqrt)
    vec4 finalColor = sqrt(ambient + light * u_lightColor + glare);
    finalColor.w = 1.0;
    gl_FragColor = finalColor;
  }
`;

AllShaders.marching_cube_vs = `
  attribute vec3 position;
  attribute vec3 normal;
  attribute vec2 texCoord;
  uniform mat4 u_worldviewproj;
  uniform mat4 u_worldview;
  uniform mat4 u_world;
  varying vec4 v_color;
  varying vec3 v_normal;
  varying vec3 v_eyeDir;
  void main(void) {
    v_color = vec4(position.x + 0.5, position.y + 0.5, position.z + 0.5, 1.0);
    v_normal = (u_world * vec4(normalize(normal), 0.0)).xyz;
    v_eyeDir = -normalize((u_worldview * vec4(position, 1.0)).xyz);
    gl_Position = u_worldviewproj * vec4(position, 1.0);
  }
`;

AllShaders.blur_fs = `
  #ifdef GL_ES
  precision mediump float;
  #endif
  varying vec2 v_texCoord;
  uniform vec2 blurSize;
  uniform vec4 subtract;
  uniform sampler2D mainSampler;
  void main() {
    vec4 sum = vec4(0.0);
    sum += texture2D(mainSampler, v_texCoord - 4.0 * blurSize) * 0.05;
    sum += texture2D(mainSampler, v_texCoord - 3.0 * blurSize) * 0.09;
    sum += texture2D(mainSampler, v_texCoord - 2.0 * blurSize) * 0.12;
    sum += texture2D(mainSampler, v_texCoord - 1.0 * blurSize) * 0.15;
    sum += texture2D(mainSampler, v_texCoord                 ) * 0.16;
    sum += texture2D(mainSampler, v_texCoord + 1.0 * blurSize) * 0.15;
    sum += texture2D(mainSampler, v_texCoord + 2.0 * blurSize) * 0.12;
    sum += texture2D(mainSampler, v_texCoord + 3.0 * blurSize) * 0.09;
    sum += texture2D(mainSampler, v_texCoord + 4.0 * blurSize) * 0.05;
    gl_FragColor = sum - subtract;
  }
`;

AllShaders.radial_vs = `
  attribute vec4 position;
  attribute vec2 texCoord;
  varying vec4 v_position;
  varying vec2 v_texCoord0, v_texCoord1, v_texCoord2, v_texCoord3;
  uniform float amount;
  void main() {
    vec2 tc = (position.xy + vec2(1.0, 1.0)) / 2.0;
    // tc.y = 1.0 - tc.y;
    v_texCoord0 = tc;
    v_texCoord1 = 0.5 + (tc-0.5) * (1.0 / (1.0 + amount));
    v_texCoord2 = 0.5 + (tc-0.5) * (1.0 / (1.0 + amount * 0.5));
    v_texCoord3 = 0.5 + (tc-0.5) * (1.0 / (1.0 + amount * 1.5));
    gl_Position = position;
  }
`;

AllShaders.radial_fs = `
  #ifdef GL_ES
  precision mediump float;
  #endif
  varying vec2 v_texCoord0, v_texCoord1, v_texCoord2, v_texCoord3;
  uniform sampler2D mainSampler;
  uniform float amount;
  uniform float glow;
  void main() {
    vec4 c1 = texture2D(mainSampler, v_texCoord0);
    vec4 c2 = texture2D(mainSampler, v_texCoord1);
    vec4 c3 = texture2D(mainSampler, v_texCoord2);
    vec4 c4 = texture2D(mainSampler, v_texCoord3);
    gl_FragColor = (c1 + c2 + c3 + c4) * glow / 4.0;
  }
`

AllShaders.copy_fs = `
  #ifdef GL_ES
  precision mediump float;
  #endif
  varying vec2 v_texCoord;
  uniform sampler2D mainSampler;
  void main() {
    gl_FragColor = texture2D(mainSampler, v_texCoord);
  }
`;

AllShaders.add_fs = `
  #ifdef GL_ES
  precision mediump float;
  #endif
  varying vec2 v_texCoord;
  uniform sampler2D mainSampler;
  uniform sampler2D secondSampler;
  void main() {
    // TODO: Gamma correct add?
    gl_FragColor = texture2D(mainSampler, v_texCoord) + texture2D(secondSampler, v_texCoord);
  }
`;