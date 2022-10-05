#version 330 core
layout(location = 0) in vec4 aPos;

uniform mat4 transform;

void main()
{
    gl_Position = transform * aPos;
}