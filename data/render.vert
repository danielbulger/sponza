#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec2 a_texCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec2 texCoords;
out vec3 position;
out vec3 normal;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    position = vec3(viewMatrix * modelMatrix *  vec4(a_position, 1.0));
    texCoords = a_texCoords;
    normal = normalize(normalMatrix * a_normal);
}