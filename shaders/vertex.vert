#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;

out vec3 Colour;
out vec2 TexPos;

uniform mat4 transform;

void main()
{
    Colour = aCol;
    TexPos = aTex;
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}