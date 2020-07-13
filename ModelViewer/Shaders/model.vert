#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;

out vec4 vColor;
out vec2 TexCoord;

void main()
{
  gl_Position = vec4(position, 1.0);
  vColor = vec4( 1.0, 0.0, 0.0, 1.0);
  TexCoord = aTexCoord;
}
