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
uniform vec4 lightPosition;

out vec2 texCoords;
out vec3 viewDirection;
out vec3 worldPosition;
out vec3 lightDirection;
out vec3 lightEyePosition;

void main()
{
	vec3 norm = normalize((viewMatrix * vec4(a_normal, 0.0)).xyz);
	vec3 tang = normalize((viewMatrix * vec4(a_tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));

	mat3 TBN = transpose(mat3(tang, bitang, norm));

	vec3 eyePosition = (viewMatrix * vec4(a_position, 1.0)).xyz;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);

	texCoords = a_texCoords;

	viewDirection = normalize(TBN * -eyePosition);
	worldPosition = vec3(modelMatrix * vec4(a_position, 1.0));

	lightEyePosition = vec3(viewMatrix * vec4(lightPosition.xyz, 1.0));
	lightDirection = normalize(TBN * (lightEyePosition - eyePosition));
}