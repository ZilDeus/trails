#version 460 core
layout (location = 0)out vec4 FragColor;
layout (location = 0)uniform sampler2D tex;
in vec2 coords;
void main()
{
  vec3 maptexture = texture(tex,coords).rgb;
  if(maptexture.r>0.0f)
    FragColor = vec4(maptexture - vec3(0.1f,0.0,0.0),0.0f);
  else
    FragColor = vec4(0.0f);
  //FragColor = vec4(maptexture,0.0f);


}
