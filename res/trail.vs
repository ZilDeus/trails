#version 460 core
layout (location = 0) in vec4 aPos;
out vec2 position;
out vec2 coord;
void main()
{
	gl_Position = vec4(aPos.xy,1.0f,1.0f);
	position = aPos.xy;
	coord = aPos.zw;
}
