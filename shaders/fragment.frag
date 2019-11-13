#version 330 core
in vec3 colours;

out vec4 FragColour;

void main()
{
	FragColour = vec4(colours, 1.0f);
}