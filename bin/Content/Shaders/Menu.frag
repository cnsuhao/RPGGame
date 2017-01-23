#version 400

uniform float time;
uniform float camvec[2];

uniform sampler2D tex;

in vec4 color;
in vec4 texcoords;

out vec4 outcolor;

vec4 blur(vec2 blurVec, int nSamples) {
  vec4 result = texture(tex, texcoords.xy);
  for (int i = 1; i < nSamples; ++i) {
    // get offset in range [-0.5, 0.5]:
    vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5);

    // sample & add to result:
    result += texture(tex, texcoords.xy + offset);
  }

  result /= float(nSamples);
  return result;
}

void main(void)
{
  if(camvec[0] != 0 || camvec[1] != 0) {
    outcolor = blur(vec2(camvec[0], camvec[1])*3.0f, 15000); // texture(tex, texcoords.xy);
  }else {
    outcolor = texture(tex, texcoords.xy);
  }
}
