#version 330 core

out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D hdrBuffer;

const float gamma = 2.2;
const float exposure = 1.0;

void main() {
	vec3 colour = texture(hdrBuffer, texCoords).rgb;

	colour = vec3(1.0) - exp(-colour * exposure);

	fragColour = vec4(pow(colour, vec3(1.0 / gamma)), 1.0);
}