#version 460 core
layout (location = 0)out vec4 FragColor;
layout (location = 0)uniform sampler2D tex;
in vec2 coords;
void main()
{
  vec3 result = vec3(0);
  int radius = 3;
  vec2 tex_offset = 1.0f / textureSize(tex, 0);
  for(int x = -radius; x < radius; x++)
    for(int y = -radius; y < radius; y++)
    {
      result += texture(tex, coords + vec2(tex_offset.x * x, tex_offset.y * y)).rgb;
    }
  if(length(result)>0.0)
  FragColor = vec4(result,0);

}
