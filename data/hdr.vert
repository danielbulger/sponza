#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoods;

out vec2 texCoords;

void main() {
	texCoords = a_texCoods;
	gl_Position = vec4(a_position, 1.0);
}