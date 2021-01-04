#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normals;
layout (location = 2) in vec2 a_texCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 texCoords;
out vec3 normals;
out vec3 fragPos;

void main()
{
    texCoords = a_texCoords.st;
    normals = a_normals;

    gl_Position = projection * view * model * vec4(a_position, 1.0);
    fragPos = vec3(a_position);
}