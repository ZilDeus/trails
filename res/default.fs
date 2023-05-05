#version 460 core
layout (location = 0)out vec4 FragColor;
layout (binding = 0)uniform sampler2D screen;
layout (binding = 1)uniform sampler2D bloom;
in vec2 coords;
void main()
{
  vec3 screenTexture = vec4(texture(bloom,coords) + texture(screen,coords)).rgb;
  if(screenTexture.x+screenTexture.y+screenTexture.z != 0)
    FragColor = vec4(screenTexture,1);
  else
    FragColor = vec4(0);

}
