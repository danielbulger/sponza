#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec2 a_texCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;

out vec2 texCoords;
out vec3 lightDirection;
out vec3 viewDirection;

void main()
{

    vec3 norm = normalize((viewMatrix * vec4(a_normal, 0.0)).xyz);
    vec3 tang = normalize((viewMatrix * vec4(a_tangent, 0.0)).xyz);
    vec3 bitang = normalize(cross(norm, tang));

    mat3 TBN = transpose(mat3(tang, bitang, norm));

    vec3 eyePosition = (viewMatrix * vec4(a_position, 1.0)).xyz;
    vec3 lightEyePosition = (viewMatrix * vec4(lightPosition, 1.0)).xyz;

    lightDirection = normalize(TBN * (lightEyePosition - eyePosition));
    viewDirection = normalize(TBN * -eyePosition);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    texCoords = a_texCoords;
}