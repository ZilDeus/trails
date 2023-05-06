#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aCord;
out vec2 coords;
void main()
{
	gl_Position = vec4(aPos.xy,1.0f,1.0f);
  coords = aCord;
}
