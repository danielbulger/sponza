#version 330 core

in vec4 position;

uniform vec4 lightPosition;
uniform float farPlane;

void main() {
	gl_FragDepth = length(position.xyz - lightPosition.xyz) / farPlane;
}