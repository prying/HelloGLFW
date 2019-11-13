#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertColours;

out vec3 colours;

void main()
{
    colours = vertColours;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}