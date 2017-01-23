#version 400

in vec3 gpu_Vertex;
in vec4 gpu_Color;
in vec4 gpu_TexCoords;
uniform mat4 gpu_ModelViewProjectionMatrix;

out vec4 color;
out vec4 texcoords;

void main(void)
{
  texcoords = gpu_TexCoords;
  color = gpu_Color;
  gl_Position = gpu_ModelViewProjectionMatrix * vec4(gpu_Vertex, 1.0);
}


