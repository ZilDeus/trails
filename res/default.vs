#version 460 core
layout (location = 0) in vec2 aPos;
out vec2 coords;
void main()
{
	gl_Position = vec4(aPos.xy,1.0f,1.0f);
  coords = (aPos+1.0f) * 0.5f;
}
