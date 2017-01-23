#version 120

uniform float time;
uniform float camvec[2];

uniform sampler2D tex;

varying vec4 color;
varying vec4 texcoords;

vec4 blur(vec2 blurVec, int nSamples) {
  vec4 result = texture2D(tex, texcoords.xy);
  for (int i = 1; i < nSamples; ++i) {
    // get offset in range [-0.5, 0.5]:
    vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5);

    // sample & add to result:
    result += texture2D(tex, texcoords.xy + offset);
  }

  result /= float(nSamples);
  return result;
}

void main(void)
{
  gl_FragColor = blur(vec2(camvec[0], camvec[1])/3.0f, 100); // texture2D(tex, texcoords.xy);
}
