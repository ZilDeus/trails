#version 460 core
layout (location = 0)out vec4 FragColor;
layout (location = 1)out vec4 BloomColor;
layout (binding = 0)uniform sampler2D tex;
in vec2 coords;
void main()
{
  FragColor =  texture(tex,coords);
  if(texture(tex,coords).x>0.05)
    BloomColor = texture(tex,coords) * 5;
  else
    BloomColor = vec4(0);

}
