#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec2 a_texCoords;

struct Light {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 coef;
};

layout (std140) uniform LightBlock {
	Light lights[4];
};

layout (std140) uniform CameraBlock
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 normalMatrix;
};

uniform mat4 modelMatrix;

out vec3 worldPosition;
out vec2 texCoords;
out vec3 viewDirection;
out vec3 lightDirection[4];

void main()
{
	vec3 norm = normalize((viewMatrix * vec4(a_normal, 0.0)).xyz);
	vec3 tang = normalize((viewMatrix * vec4(a_tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));

	mat3 TBN = transpose(mat3(tang, bitang, norm));

	vec3 eyePosition = (viewMatrix * vec4(a_position, 1.0)).xyz;

	for (int i = 0; i < 4; ++i) {
		vec3 lightEyePosition = (viewMatrix * vec4(lights[i].position.xyz, 1.0)).xyz;
		lightDirection[i] = normalize(TBN * (lightEyePosition - eyePosition));
	}

	viewDirection = normalize(TBN * -eyePosition);

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
	worldPosition = a_position;
	texCoords = a_texCoords;
}