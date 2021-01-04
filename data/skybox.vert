#version 330 core

layout (location=0) in vec3 a_position;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texCoords = a_position;
    gl_Position = (projection * view * vec4(a_position, 1.0)).xyww;
}