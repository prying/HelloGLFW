#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    TexPos = aTex;
    gl_Position =  proj * view * model * vec4(aPos, 1.0);
}