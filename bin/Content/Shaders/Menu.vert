#version 120

attribute vec3 gpu_Vertex;
attribute vec4 gpu_Color;
attribute vec4 gpu_TexCoords;
uniform mat4 gpu_ModelViewProjectionMatrix;

varying vec4 color;
varying vec4 texcoords;

void main(void)
{
  texcoords = gpu_TexCoords;
  color = gpu_Color;
  gl_Position = gpu_ModelViewProjectionMatrix * vec4(gpu_Vertex, 1.0);
}


