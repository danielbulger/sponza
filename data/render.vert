#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec2 a_texCoords;

layout (std140) uniform CameraBlock
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 normalMatrix;
};

uniform mat4 modelMatrix;

out vec2 texCoords;
out vec3 position;
out vec3 normal;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
	position = vec3(viewMatrix * modelMatrix *  vec4(a_position, 1.0));
	texCoords = a_texCoords;
	normal = normalize(mat3(normalMatrix) * a_normal);
}