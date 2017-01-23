#version 120

uniform sampler2D tex;

varying vec4 color;
varying vec4 texcoords;

void main(void)
{
  gl_FragColor = texture2D(tex, texcoords.xy);
}
