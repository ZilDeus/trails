#version 460 core
layout (location = 0) in vec2 aPos;
uniform vec2 uPos;
void main()
{
	gl_Position = vec4(uPos.xy,0.0f,1.0f);
}
