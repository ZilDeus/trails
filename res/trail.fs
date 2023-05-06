#version 460 core
in vec2 coord;
layout (location = 0)out vec4 FragColor;
layout (location = 0)uniform sampler2D tex;
void main()
{
  FragColor =  vec4(texture(tex,coord).rgb * coord.y,coord.y);
}
