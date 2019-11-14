#version 330 core
in vec3 colour;
in vec2 TexCoord;

out vec4 FragColour;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float alphaMix;

void main()
{
	FragColour = mix(texture(texture1, TexCoord),texture(texture2, TexCoord), alphaMix) * vec4(colour, 1.0f);
	//FragColour = vec4(colour, 1.0f);
}