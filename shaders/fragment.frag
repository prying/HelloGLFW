#version 330 core
in vec3 Colour;
in vec2 TexPos;

out vec4 FragColour;

uniform sampler2D texture1;

void main()
{
	FragColour = texture(texture1, TexPos) * vec4(Colour, 1.0f);
}