#version 330 core

struct Light {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 coef;
};

layout (std140) uniform LightBlock
{
	Light lights[4];
};

in vec2 texCoords;
in vec3 viewDirection;
in vec3 lightPositions[4];
in vec3 worldPosition;

out vec4 fragColour;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform bool alpha;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

const float gamma = 2.2;

vec3 calculateLight(Light light, vec3 lightEyePosition, vec3 normal)
{
	float distance = length(light.position.xyz - worldPosition);
	float attenuation = 5000 / (light.coef.x + light.coef.y * distance + light.coef.z * (distance * distance));

	float dotN = max(dot(normal, lightEyePosition), 0.0);
	vec3 reflectedLightDirection = reflect(-lightEyePosition, normal);
	float specularFactor = max(dot(reflectedLightDirection, viewDirection), 0.0);

	vec3 ka = mix(light.ambient.xyz, ambient, 0.5) * vec3(texture(ambientTexture, texCoords));
	vec3 kd = light.diffuse.xyz * diffuse * vec3(texture(diffuseTexture, texCoords)) * dotN;
	vec3 ks = light.specular.xyz * specular * vec3(texture(specularTexture, texCoords)) * pow(specularFactor, specularExponent);

	return attenuation * (ka + (kd +  ks));
}

void main()
{
	if (alpha && texture(alphaTexture, texCoords).r == 0) {
		discard;
	}

	// Map into -1 to 1 range.
	vec3 normal = (2.0 * texture(normalTexture, texCoords, -1.0) - 1.0).rgb;

	vec3 colour = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < 4; ++i) {
		colour += calculateLight(lights[i], lightPositions[i], normal);
	}

	fragColour = vec4(colour, 1.0);
}